/*
 * mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Based on mbed-stress-test by Marcus Chang @ Arm Mbed - http://github.com/ARMmbed/mbed-stress-test
*/

#include "mbed.h"
#include "unity/unity.h"
#include "download_test.h"
#include <string>

#include "alice.h"

#define MAX_RETRIES 3
NetworkInterface* net = NULL;
static bool setup_network() {
    net = NetworkInterface::get_default_instance();
    if(net == NULL) { printf("[ERROR] failed to initialize network\n"); return false; }

    nsapi_error_t err = -1;
    for (int tries = 0; tries < MAX_RETRIES; tries++) {
        err = net->connect();
        if (err == NSAPI_ERROR_OK) {
            break;
        } else {
            printf("[ERROR] Connecting to network. Retrying %d of %d\n", tries, MAX_RETRIES);
        }
    }
    printf("[NET] err=%d\n", err);
    printf("[NET] IP address is '%s'\n", net->get_ip_address());
    printf("[NET] MAC address is '%s'\n", net->get_mac_address());
    
    return err == NSAPI_ERROR_OK;
}

static uint32_t thread_counter = 0;

void download_fn() {
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    download_test(net, story, sizeof(story), 1024, thread_id);
}

int main()
{
    printf("[ALL] App start\n");

    if (setup_network()) {
        Thread t1;
        Thread t2;
        Thread t3;
        Thread t4;
        t1.start(download_fn);
        t2.start(download_fn);
        t3.start(download_fn);
        t4.start(download_fn);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }

    printf("[ALL] Done\n");
    while (true);
}
