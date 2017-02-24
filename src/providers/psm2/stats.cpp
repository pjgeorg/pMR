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

#include "stats.hpp"
#include "../../misc/print.hpp"

void pMR::psm2::printStats(psm2_mq_t mq)
{
    psm2_mq_stats stats;

    psm2_mq_get_stats(mq, &stats);

    print("PSM2 MatchedQueue Stats:",
        "\nBytes received into a matched user buffer:        ",
        stats.rx_user_bytes,
        "\nMessages received into a matched user buffer:     ",
        stats.rx_user_num,
        "\nBytes received into an unmatched system buffer:   ",
        stats.rx_sys_bytes,
        "\nMessages received into an unmatched system buffer:",
        stats.rx_sys_num,
        "\nTotal messages transmitted (SHM and HFI):         ", stats.tx_num,
        "\nMessages transmitted eagerly:                     ",
        stats.tx_eager_num,
        "\nBytes transmitted eagerly:                        ",
        stats.tx_eager_bytes,
        "\nMessages transmitted using expected TID mechanism:",
        stats.tx_rndv_num,
        "\nBytes transmitted using excpected TID mechanism:  ",
        stats.tx_rndv_bytes,
        "\nMessages transmitted (SHM only):                  ",
        stats.tx_shm_num,
        "\nMessages received (SHM only):                     ",
        stats.rx_shm_num,
        "\nNumber of allocated system buffers:               ",
        stats.rx_sysbuf_num,
        "\nBytes allocated for system buffers:               ",
        stats.rx_sysbuf_bytes);
}
