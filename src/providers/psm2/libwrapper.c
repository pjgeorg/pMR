//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <psm2.h>
#include <psm2_am.h>
#include <psm2_mq.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t gMutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static void __psm2_lock()
{
    if(pthread_mutex_lock(&gMutex) != 0)
    {
        fprintf(stderr, "PSM2 wrapper unable to acquire lock.");
        exit(-1);
    }
}

static void __psm2_unlock()
{
    if(pthread_mutex_unlock(&gMutex) != 0)
    {
        fprintf(stderr, "PSM2 wrapper unable to release lock.");
        exit(-1);
    }
}

static psm2_ep_t gEndpoint;
static psm2_epid_t gEndpointID;
static psm2_mq_t gMatchedQueue;

static psm2_error_t (*orig_psm2_init)(
    int *api_verno_major, int *api_verno_minor);
static psm2_error_t (*orig_psm2_finalize)(void);
static psm2_error_t (*orig_psm2_error_register_handler)(
    psm2_ep_t ep, const psm2_ep_errhandler_t errhandler);
static psm2_error_t (*orig_psm2_error_defer)(psm2_error_token_t err_token);
static uint64_t (*orig_psm2_epid_nid)(psm2_epid_t epid);
static uint64_t (*orig_psm2_epid_context)(psm2_epid_t epid);
static uint64_t (*orig_psm2_epid_port)(psm2_epid_t epid);
static psm2_error_t (*orig_psm2_ep_num_devunits)(uint32_t *num_units);
static void (*orig_psm2_uuid_generate)(psm2_uuid_t uuid_out);
static psm2_error_t (*orig_psm2_ep_open)(const psm2_uuid_t unique_job_key,
    const struct psm2_ep_open_opts *opts, psm2_ep_t *ep, psm2_epid_t *epid);
static psm2_error_t (*orig_psm2_ep_open_opts_get_defaults)(
    struct psm2_ep_open_opts *opts);
static psm2_error_t (*orig_psm2_ep_epid_share_memory)(
    psm2_ep_t ep, psm2_epid_t epid, int *result);
static psm2_error_t (*orig_psm2_ep_close)(
    psm2_ep_t ep, int mode, int64_t timeout);
static psm2_error_t (*orig_psm2_map_nid_hostname)(
    int num, const uint64_t *nids, const char **hostnames);
static psm2_error_t (*orig_psm2_ep_connect)(psm2_ep_t ep, int num_of_epid,
    const psm2_epid_t *array_of_epid, const int *array_of_epid_mask,
    psm2_error_t *array_of_errors, psm2_epaddr_t *array_of_epaddr,
    int64_t timeout);
static psm2_error_t (*orig_psm2_poll)(psm2_ep_t ep);
static void (*orig_psm2_epaddr_setlabel)(
    psm2_epaddr_t epaddr, const char *epaddr_label_string);
static void (*orig_psm2_epaddr_setctxt)(psm2_epaddr_t epaddr, void *ctxt);
static void *(*orig_psm2_epaddr_getctxt)(psm2_epaddr_t epaddr);
static psm2_error_t (*orig_psm2_setopt)(psm2_component_t component,
    const void *component_obj, int optname, const void *optval,
    uint64_t optlen);
static psm2_error_t (*orig_psm2_getopt)(psm2_component_t component,
    const void *component_obj, int optname, void *optval, uint64_t *optlen);
static psm2_error_t (*orig_psm2_ep_query)(
    int *num_of_epinfo, psm2_epinfo_t *array_of_epinfo);
static psm2_error_t (*orig_psm2_ep_epid_lookup)(
    psm2_epid_t epid, psm2_epconn_t *epconn);

static psm2_error_t (*orig_psm2_mq_init)(psm2_ep_t ep, uint64_t tag_order_mask,
    const struct psm2_optkey *opts, int numopts, psm2_mq_t *mq);
static psm2_error_t (*orig_psm2_mq_finalize)(psm2_mq_t mq);
static psm2_error_t (*orig_psm2_mq_getopt)(
    psm2_mq_t mq, int option, void *value);
static psm2_error_t (*orig_psm2_mq_setopt)(
    psm2_mq_t mq, int option, const void *value);
static psm2_error_t (*orig_psm2_mq_irecv)(psm2_mq_t mq, uint64_t rtag,
    uint64_t rtagsel, uint32_t flags, void *buf, uint32_t len, void *context,
    psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_irecv2)(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, uint32_t flags, void *buf,
    uint32_t len, void *context, psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_imrecv)(psm2_mq_t mq, uint32_t flags,
    void *buf, uint32_t len, void *context, psm2_mq_req_t *reqo);
static psm2_error_t (*orig_psm2_mq_send)(psm2_mq_t mq, psm2_epaddr_t dest,
    uint32_t flags, uint64_t stag, const void *buf, uint32_t len);
static psm2_error_t (*orig_psm2_mq_send2)(psm2_mq_t mq, psm2_epaddr_t dest,
    uint32_t flags, psm2_mq_tag_t *stag, const void *buf, uint32_t len);
static psm2_error_t (*orig_psm2_mq_isend)(psm2_mq_t mq, psm2_epaddr_t dest,
    uint32_t flags, uint64_t stag, const void *buf, uint32_t len, void *context,
    psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_isend2)(psm2_mq_t mq, psm2_epaddr_t dest,
    uint32_t flags, psm2_mq_tag_t *stag, const void *buf, uint32_t len,
    void *context, psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_iprobe)(
    psm2_mq_t mq, uint64_t rtag, uint64_t rtagsel, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_iprobe2)(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_improbe)(psm2_mq_t mq, uint64_t rtag,
    uint64_t rtagsel, psm2_mq_req_t *req, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_improbe2)(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, psm2_mq_req_t *reqo,
    psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_ipeek)(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_ipeek2)(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_wait)(
    psm2_mq_req_t *request, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_wait2)(
    psm2_mq_req_t *request, psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_test)(
    psm2_mq_req_t *request, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_test2)(
    psm2_mq_req_t *request, psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_cancel)(psm2_mq_req_t *req);

static psm2_error_t (*orig_psm2_am_register_handlers)(psm2_ep_t ep,
    const psm2_am_handler_fn_t *handlers, int num_handlers, int *handlers_idx);
static psm2_error_t (*orig_psm2_am_request_short)(psm2_epaddr_t epaddr,
    psm2_handler_t handler, psm2_amarg_t *args, int nargs, void *src,
    size_t len, int flags, psm2_am_completion_fn_t completion_fn,
    void *completion_ctxt);
static psm2_error_t (*orig_psm2_am_reply_short)(psm2_am_token_t token,
    psm2_handler_t handler, psm2_amarg_t *args, int nargs, void *src,
    size_t len, int flags, psm2_am_completion_fn_t completion_fn,
    void *completion_ctxt);
static psm2_error_t (*orig_psm2_am_get_source)(
    psm2_am_token_t token, psm2_epaddr_t *epaddr_out);
static psm2_error_t (*orig_psm2_am_get_parameters)(psm2_ep_t ep,
    struct psm2_am_parameters *parameters, size_t sizeof_parameters_in,
    size_t *sizeof_parameters_out);

__attribute__((constructor)) static void __psm2_constructor(void)
{
    orig_psm2_init = dlsym(RTLD_NEXT, "psm2_init");
    orig_psm2_finalize = dlsym(RTLD_NEXT, "psm2_finalize");
    orig_psm2_error_register_handler =
        dlsym(RTLD_NEXT, "psm2_error_register_handler");
    orig_psm2_error_defer = dlsym(RTLD_NEXT, "psm2_error_defer");
    orig_psm2_epid_nid = dlsym(RTLD_NEXT, "psm2_epid_nid");
    orig_psm2_epid_context = dlsym(RTLD_NEXT, "psm2_epid_context");
    orig_psm2_epid_port = dlsym(RTLD_NEXT, "psm2_epid_port");
    orig_psm2_ep_num_devunits = dlsym(RTLD_NEXT, "psm2_ep_num_devunits");
    orig_psm2_uuid_generate = dlsym(RTLD_NEXT, "psm2_uuid_generate");
    orig_psm2_ep_open = dlsym(RTLD_NEXT, "psm2_ep_open");
    orig_psm2_ep_open_opts_get_defaults =
        dlsym(RTLD_NEXT, "psm2_ep_open_opts_get_defaults");
    orig_psm2_ep_epid_share_memory =
        dlsym(RTLD_NEXT, "psm2_ep_epid_share_memory");
    orig_psm2_ep_close = dlsym(RTLD_NEXT, "psm2_ep_close");
    orig_psm2_map_nid_hostname = dlsym(RTLD_NEXT, "psm2_map_nid_hostname");
    orig_psm2_ep_connect = dlsym(RTLD_NEXT, "psm2_ep_connect");
    orig_psm2_poll = dlsym(RTLD_NEXT, "psm2_poll");
    orig_psm2_epaddr_setlabel = dlsym(RTLD_NEXT, "psm2_epaddr_setlabel");
    orig_psm2_epaddr_setctxt = dlsym(RTLD_NEXT, "psm2_epaddr_setctxt");
    orig_psm2_epaddr_getctxt = dlsym(RTLD_NEXT, "psm2_epaddr_getctxt");
    orig_psm2_setopt = dlsym(RTLD_NEXT, "psm2_setopt");
    orig_psm2_getopt = dlsym(RTLD_NEXT, "psm2_getopt");
    orig_psm2_ep_query = dlsym(RTLD_NEXT, "psm2_ep_query");
    orig_psm2_ep_epid_lookup = dlsym(RTLD_NEXT, "psm2_ep_epid_lookup");

    orig_psm2_mq_init = dlsym(RTLD_NEXT, "psm2_mq_init");
    orig_psm2_mq_finalize = dlsym(RTLD_NEXT, "psm2_mq_finalize");
    orig_psm2_mq_getopt = dlsym(RTLD_NEXT, "psm2_mq_getopt");
    orig_psm2_mq_setopt = dlsym(RTLD_NEXT, "psm2_mq_setopt");
    orig_psm2_mq_irecv = dlsym(RTLD_NEXT, "psm2_mq_irecv");
    orig_psm2_mq_irecv2 = dlsym(RTLD_NEXT, "psm2_mq_irecv2");
    orig_psm2_mq_imrecv = dlsym(RTLD_NEXT, "psm2_mq_imrecv");
    orig_psm2_mq_send = dlsym(RTLD_NEXT, "psm2_mq_send");
    orig_psm2_mq_send2 = dlsym(RTLD_NEXT, "psm2_mq_send2");
    orig_psm2_mq_isend = dlsym(RTLD_NEXT, "psm2_mq_isend");
    orig_psm2_mq_isend2 = dlsym(RTLD_NEXT, "psm2_mq_isend2");
    orig_psm2_mq_iprobe = dlsym(RTLD_NEXT, "psm2_mq_iprobe");
    orig_psm2_mq_iprobe2 = dlsym(RTLD_NEXT, "psm2_mq_iprobe2");
    orig_psm2_mq_improbe = dlsym(RTLD_NEXT, "psm2_mq_improbe");
    orig_psm2_mq_improbe2 = dlsym(RTLD_NEXT, "psm2_mq_improbe2");
    orig_psm2_mq_ipeek = dlsym(RTLD_NEXT, "psm2_mq_ipeek");
    orig_psm2_mq_ipeek2 = dlsym(RTLD_NEXT, "psm2_mq_ipeek2");
    orig_psm2_mq_wait = dlsym(RTLD_NEXT, "psm2_mq_wait");
    orig_psm2_mq_wait2 = dlsym(RTLD_NEXT, "psm2_mq_wait2");
    orig_psm2_mq_test = dlsym(RTLD_NEXT, "psm2_mq_test");
    orig_psm2_mq_test2 = dlsym(RTLD_NEXT, "psm2_mq_test2");
    orig_psm2_mq_cancel = dlsym(RTLD_NEXT, "psm2_mq_cancel");

    orig_psm2_am_register_handlers =
        dlsym(RTLD_NEXT, "psm2_am_register_handlers");
    orig_psm2_am_request_short = dlsym(RTLD_NEXT, "psm2_am_request_short");
    orig_psm2_am_reply_short = dlsym(RTLD_NEXT, "psm2_am_reply_short");
    orig_psm2_am_get_source = dlsym(RTLD_NEXT, "psm2_am_get_source");
    orig_psm2_am_get_parameters = dlsym(RTLD_NEXT, "psm2_am_get_parameters");
}

psm2_error_t psm2_init(int *api_verno_major, int *api_verno_minor)
{
    __psm2_lock();

    static int sInitialized = 0;
    psm2_error_t err = PSM2_OK;
    if(!sInitialized)
    {
        int vMajor = PSM2_VERNO_MAJOR;
        int vMinor = PSM2_VERNO_MINOR;
        err = orig_psm2_init(&vMajor, &vMinor);
        sInitialized = 1;
    }

    __psm2_unlock();
    return err;
}

psm2_error_t psm2_finalize(void)
{
    return PSM2_OK;
}

psm2_error_t psm2_error_register_handler(
    psm2_ep_t ep, const psm2_ep_errhandler_t errhandler)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_error_register_handler(ep, errhandler);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_error_defer(psm2_error_token_t err_token)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_error_defer(err_token);
    __psm2_unlock();
    return err;
}

uint64_t psm2_epid_nid(psm2_epid_t epid)
{
    __psm2_lock();
    uint64_t id = orig_psm2_epid_nid(epid);
    __psm2_unlock();
    return id;
}

uint64_t psm2_epid_context(psm2_epid_t epid)
{
    __psm2_lock();
    uint64_t id = orig_psm2_epid_context(epid);
    __psm2_unlock();
    return id;
}

uint64_t psm2_epid_port(psm2_epid_t epid)
{
    __psm2_lock();
    uint64_t id = orig_psm2_epid_port(epid);
    __psm2_unlock();
    return id;
}

psm2_error_t psm2_ep_num_devunits(uint32_t *num_units)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_num_devunits(num_units);
    __psm2_unlock();
    return err;
}

void psm2_uuid_generate(psm2_uuid_t uuid_out)
{
    __psm2_lock();
    orig_psm2_uuid_generate(uuid_out);
    __psm2_unlock();
    return;
}

psm2_error_t psm2_ep_open(const psm2_uuid_t unique_job_key,
    const struct psm2_ep_open_opts *opts, psm2_ep_t *ep, psm2_epid_t *epid)
{
    __psm2_lock();

    static int sInitialized = 0;
    psm2_error_t err = PSM2_OK;
    if(!sInitialized)
    {
        sInitialized = 1;
        err = orig_psm2_ep_open(unique_job_key, opts, &gEndpoint, &gEndpointID);
    }
    *ep = gEndpoint;
    *epid = gEndpointID;

    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_open_opts_get_defaults(struct psm2_ep_open_opts *opts)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_open_opts_get_defaults(opts);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_epid_share_memory(
    psm2_ep_t ep, psm2_epid_t epid, int *result)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_epid_share_memory(ep, epid, result);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_close(psm2_ep_t ep, int mode, int64_t timeout)
{
    return PSM2_OK;
}

psm2_error_t psm2_map_nid_hostname(
    int num, const uint64_t *nids, const char **hostnames)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_map_nid_hostname(num, nids, hostnames);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_connect(psm2_ep_t ep, int num_of_epid,
    const psm2_epid_t *array_of_epid, const int *array_of_epid_mask,
    psm2_error_t *array_of_errors, psm2_epaddr_t *array_of_epaddr,
    int64_t timeout)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_connect(ep, num_of_epid, array_of_epid,
        array_of_epid_mask, array_of_errors, array_of_epaddr, timeout);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_poll(psm2_ep_t ep)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_poll(ep);
    __psm2_unlock();
    return err;
}

void psm2_epaddr_setlabel(psm2_epaddr_t epaddr, const char *epaddr_label_string)
{
    __psm2_lock();
    orig_psm2_epaddr_setlabel(epaddr, epaddr_label_string);
    __psm2_unlock();
    return;
}

void psm2_epaddr_setctxt(psm2_epaddr_t epaddr, void *ctxt)
{
    __psm2_lock();
    orig_psm2_epaddr_setctxt(epaddr, ctxt);
    __psm2_unlock();
    return;
}

void *psm2_epaddr_getctxt(psm2_epaddr_t epaddr)
{
    __psm2_lock();
    void *ctxt = orig_psm2_epaddr_getctxt(epaddr);
    __psm2_unlock();
    return ctxt;
}

psm2_error_t psm2_setopt(psm2_component_t component, const void *component_obj,
    int optname, const void *optval, uint64_t optlen)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_setopt(component, component_obj, optname, optval, optlen);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_getopt(psm2_component_t component, const void *component_obj,
    int optname, void *optval, uint64_t *optlen)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_getopt(component, component_obj, optname, optval, optlen);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_query(int *num_of_epinfo, psm2_epinfo_t *array_of_epinfo)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_query(num_of_epinfo, array_of_epinfo);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_ep_epid_lookup(psm2_epid_t epid, psm2_epconn_t *epconn)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_ep_epid_lookup(epid, epconn);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_init(psm2_ep_t ep, uint64_t tag_order_mask,
    const struct psm2_optkey *opts, int numopts, psm2_mq_t *mq)
{
    __psm2_lock();

    static int sInitialized = 0;
    psm2_error_t err = PSM2_OK;
    if(!sInitialized)
    {
        sInitialized = 1;
        err = orig_psm2_mq_init(
            ep, tag_order_mask, opts, numopts, &gMatchedQueue);
    }
    *mq = gMatchedQueue;

    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_finalize(psm2_mq_t mq)
{
    return PSM2_OK;
}

psm2_error_t psm2_mq_getopt(psm2_mq_t mq, int option, void *value)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_getopt(mq, option, value);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_setopt(psm2_mq_t mq, int option, const void *value)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_setopt(mq, option, value);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_irecv(psm2_mq_t mq, uint64_t rtag, uint64_t rtagsel,
    uint32_t flags, void *buf, uint32_t len, void *context, psm2_mq_req_t *req)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_mq_irecv(mq, rtag, rtagsel, flags, buf, len, context, req);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_irecv2(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, uint32_t flags, void *buf,
    uint32_t len, void *context, psm2_mq_req_t *req)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_irecv2(
        mq, src, rtag, rtagsel, flags, buf, len, context, req);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_imrecv(psm2_mq_t mq, uint32_t flags, void *buf,
    uint32_t len, void *context, psm2_mq_req_t *reqo)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_imrecv(mq, flags, buf, len, context, reqo);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_send(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
    uint64_t stag, const void *buf, uint32_t len)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_send(mq, dest, flags, stag, buf, len);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_send2(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
    psm2_mq_tag_t *stag, const void *buf, uint32_t len)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_send2(mq, dest, flags, stag, buf, len);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_isend(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
    uint64_t stag, const void *buf, uint32_t len, void *context,
    psm2_mq_req_t *req)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_mq_isend(mq, dest, flags, stag, buf, len, context, req);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_isend2(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
    psm2_mq_tag_t *stag, const void *buf, uint32_t len, void *context,
    psm2_mq_req_t *req)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_mq_isend2(mq, dest, flags, stag, buf, len, context, req);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_iprobe(
    psm2_mq_t mq, uint64_t rtag, uint64_t rtagsel, psm2_mq_status_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_iprobe(mq, rtag, rtagsel, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_iprobe2(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, psm2_mq_status2_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_iprobe2(mq, src, rtag, rtagsel, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_improbe(psm2_mq_t mq, uint64_t rtag, uint64_t rtagsel,
    psm2_mq_req_t *req, psm2_mq_status_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_improbe(mq, rtag, rtagsel, req, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_improbe2(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, psm2_mq_req_t *reqo,
    psm2_mq_status2_t *status)
{
    __psm2_lock();
    psm2_error_t err =
        orig_psm2_mq_improbe2(mq, src, rtag, rtagsel, reqo, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_ipeek(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_ipeek(mq, req, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_ipeek2(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status2_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_ipeek2(mq, req, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_wait(psm2_mq_req_t *request, psm2_mq_status_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_wait(request, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_wait2(psm2_mq_req_t *request, psm2_mq_status2_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_wait2(request, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_test(psm2_mq_req_t *request, psm2_mq_status_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_test(request, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_test2(psm2_mq_req_t *request, psm2_mq_status2_t *status)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_test2(request, status);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_cancel(psm2_mq_req_t *req)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_mq_cancel(req);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_am_register_handlers(psm2_ep_t ep,
    const psm2_am_handler_fn_t *handlers, int num_handlers, int *handlers_idx)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_am_register_handlers(
        ep, handlers, num_handlers, handlers_idx);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_am_request_short(psm2_epaddr_t epaddr, psm2_handler_t handler,
    psm2_amarg_t *args, int nargs, void *src, size_t len, int flags,
    psm2_am_completion_fn_t completion_fn, void *completion_ctxt)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_am_request_short(epaddr, handler, args, nargs,
        src, len, flags, completion_fn, completion_ctxt);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_am_reply_short(psm2_am_token_t token, psm2_handler_t handler,
    psm2_amarg_t *args, int nargs, void *src, size_t len, int flags,
    psm2_am_completion_fn_t completion_fn, void *completion_ctxt)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_am_reply_short(token, handler, args, nargs,
        src, len, flags, completion_fn, completion_ctxt);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_am_get_source(
    psm2_am_token_t token, psm2_epaddr_t *epaddr_out)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_am_get_source(token, epaddr_out);
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_am_get_parameters(psm2_ep_t ep,
    struct psm2_am_parameters *parameters, size_t sizeof_parameters_in,
    size_t *sizeof_parameters_out)
{
    __psm2_lock();
    psm2_error_t err = orig_psm2_am_get_parameters(
        ep, parameters, sizeof_parameters_in, sizeof_parameters_out);
    __psm2_unlock();
    return err;
}
