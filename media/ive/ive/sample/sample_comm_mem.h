/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _SAMPLE_COMM_MEM_H_
#define _SAMPLE_COMM_MEM_H_

#define MmzAlloc SYS_MmzAlloc
#define MmzAlloc_Cached SYS_MmzAlloc_Cached
#define MmzFree SYS_MmzFree

int SYS_MmzAlloc(void* phy_addr, void** vir_addr, unsigned int size);
int SYS_MmzAlloc_Cached(void* phy_addr, void** vir_addr, unsigned int size);
int SYS_MmzFlush_Start(void* vir_addr);
int SYS_MmzFlush_End(void* vir_addr);
int SYS_MmzFree(void* phy_addr);
#endif /*_SAMPLE_COMM_MEM_H_*/
