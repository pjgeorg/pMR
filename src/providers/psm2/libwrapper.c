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
#include <psm2_mq.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

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
static psm2_error_t (*orig_psm2_ep_open)(const psm2_uuid_t unique_job_key,
    const struct psm2_ep_open_opts *opts, psm2_ep_t *ep, psm2_epid_t *epid);
static psm2_error_t (*orig_psm2_ep_close)(
    psm2_ep_t ep, int mode, int64_t timeout);

static psm2_error_t (*orig_psm2_mq_init)(psm2_ep_t ep, uint64_t tag_order_mask,
    const struct psm2_optkey *opts, int numopts, psm2_mq_t *mq);
static psm2_error_t (*orig_psm2_mq_finalize)(psm2_mq_t mq);

static psm2_error_t (*orig_psm2_mq_ipeek)(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status_t *status);
static psm2_error_t (*orig_psm2_mq_ipeek2)(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status2_t *status);

#ifdef PSM2_WRAPPER_BLOCK
static int sLocked = 0;
static psm2_error_t (*orig_psm2_mq_irecv2)(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, uint32_t flags, void *buf,
    uint32_t len, void *context, psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_isend2)(psm2_mq_t mq, psm2_epaddr_t dest,
    uint32_t flags, psm2_mq_tag_t *stag, const void *buf, uint32_t len,
    void *context, psm2_mq_req_t *req);
static psm2_error_t (*orig_psm2_mq_wait2)(
    psm2_mq_req_t *request, psm2_mq_status2_t *status);
static psm2_error_t (*orig_psm2_mq_test2)(
    psm2_mq_req_t *request, psm2_mq_status2_t *status);
#endif // PSM2_WRAPPER_BLOCK

__attribute__((constructor)) static void __psm2_constructor(void)
{
    orig_psm2_init = dlsym(RTLD_NEXT, "psm2_init");
    orig_psm2_finalize = dlsym(RTLD_NEXT, "psm2_finalize");
    orig_psm2_ep_open = dlsym(RTLD_NEXT, "psm2_ep_open");
    orig_psm2_ep_close = dlsym(RTLD_NEXT, "psm2_ep_close");

    orig_psm2_mq_init = dlsym(RTLD_NEXT, "psm2_mq_init");
    orig_psm2_mq_finalize = dlsym(RTLD_NEXT, "psm2_mq_finalize");

    orig_psm2_mq_ipeek = dlsym(RTLD_NEXT, "psm2_mq_ipeek");
    orig_psm2_mq_ipeek2 = dlsym(RTLD_NEXT, "psm2_mq_ipeek2");

#ifdef PSM2_WRAPPER_BLOCK
    orig_psm2_mq_irecv2 = dlsym(RTLD_NEXT, "psm2_mq_irecv2");
    orig_psm2_mq_isend2 = dlsym(RTLD_NEXT, "psm2_mq_isend2");
    orig_psm2_mq_wait2 = dlsym(RTLD_NEXT, "psm2_mq_wait2");
    orig_psm2_mq_test2 = dlsym(RTLD_NEXT, "psm2_mq_test2");
#endif // PSM2_WRAPPER_BLOCK
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

psm2_error_t psm2_ep_close(psm2_ep_t ep, int mode, int64_t timeout)
{
    return PSM2_OK;
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

#ifdef PSM2_WRAPPER_BLOCK
psm2_error_t psm2_mq_ipeek(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status_t *status)
{
    psm2_error_t err;
    __psm2_lock();
    if(sLocked == 0)
    {
        err = orig_psm2_mq_ipeek(mq, req, status);
    }
    else
    {
        err = PSM2_MQ_NO_COMPLETIONS;
    }
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_ipeek2(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status2_t *status)
{
    psm2_error_t err;
    __psm2_lock();
    if(sLocked == 0)
    {
        err = orig_psm2_mq_ipeek2(mq, req, status);
    }
    else
    {
        err = PSM2_MQ_NO_COMPLETIONS;
    }
    __psm2_unlock();
    return err;
}

psm2_error_t psm2_mq_irecv2(psm2_mq_t mq, psm2_epaddr_t src,
    psm2_mq_tag_t *rtag, psm2_mq_tag_t *rtagsel, uint32_t flags, void *buf,
    uint32_t len, void *context, psm2_mq_req_t *req)
{
    if(rtag->tag2 == 0xFFFFFFFF)
    {
        __psm2_lock();
        ++sLocked;
        __psm2_unlock();
    }
    psm2_error_t err = orig_psm2_mq_irecv2(
        mq, src, rtag, rtagsel, flags, buf, len, context, req);
    return err;
}

psm2_error_t psm2_mq_isend2(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
    psm2_mq_tag_t *stag, const void *buf, uint32_t len, void *context,
    psm2_mq_req_t *req)
{
    if(stag->tag2 == 0xFFFFFFFF)
    {
        __psm2_lock();
        ++sLocked;
        __psm2_unlock();
    }
    psm2_error_t err =
        orig_psm2_mq_isend2(mq, dest, flags, stag, buf, len, context, req);
    return err;
}

psm2_error_t psm2_mq_wait2(psm2_mq_req_t *request, psm2_mq_status2_t *status)
{
    psm2_error_t err = orig_psm2_mq_wait2(request, status);
    if(status->msg_tag.tag2 == 0xFFFFFFFF)
    {
        __psm2_lock();
        --sLocked;
        __psm2_unlock();
    }
    return err;
}

psm2_error_t psm2_mq_test2(psm2_mq_req_t *request, psm2_mq_status2_t *status)
{
    psm2_error_t err = orig_psm2_mq_test2(request, status);
    if(err == PSM2_OK)
    {
        if(status->msg_tag.tag2 == 0xFFFFFFFF)
        {
            __psm2_lock();
            --sLocked;
            __psm2_unlock();
        }
    }
    return err;
}

#else
psm2_error_t psm2_mq_ipeek(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status_t *status)
{
    psm2_error_t err;
    psm2_mq_status2_t reqStatus;
    err = orig_psm2_mq_ipeek2(mq, req, &reqStatus);
    if(err == PSM2_OK)
    {
        if(reqStatus.msg_tag.tag2 == 0xFFFFFFFF)
        {
            *req = PSM2_MQ_REQINVALID;
            err = PSM2_MQ_NO_COMPLETIONS;
        }
        else
        {
            if(status)
            {
                status->msg_tag = *((uint64_t *)reqStatus.msg_tag.tag);
                status->msg_length = reqStatus.msg_length;
                status->nbytes = reqStatus.nbytes;
                status->error_code = reqStatus.error_code;
                status->context = reqStatus.context;
            }
        }
    }
    return err;
}

psm2_error_t psm2_mq_ipeek2(
    psm2_mq_t mq, psm2_mq_req_t *req, psm2_mq_status2_t *status)
{
    psm2_error_t err;
    if(status)
    {
        err = orig_psm2_mq_ipeek2(mq, req, status);
        if(err == PSM2_OK)
        {
            if(status->msg_tag.tag2 == 0xFFFFFFFF)
            {
                *req = PSM2_MQ_REQINVALID;
                err = PSM2_MQ_NO_COMPLETIONS;
            }
        }
    }
    else
    {
        psm2_mq_status2_t reqStatus;
        err = orig_psm2_mq_ipeek2(mq, req, &reqStatus);
        if(err == PSM2_OK)
        {
            if(reqStatus.msg_tag.tag2 == 0xFFFFFFFF)
            {
                *req = PSM2_MQ_REQINVALID;
                err = PSM2_MQ_NO_COMPLETIONS;
            }
        }
    }
    return err;
}
#endif // PSM2_WRAPPER_BLOCK
