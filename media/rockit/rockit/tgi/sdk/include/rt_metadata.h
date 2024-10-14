/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
 * author: Rimon.Xu@rock-chips.com
 *   date: 20181205
 */

#ifndef INCLUDE_RT_BASE_RT_METADATA_H_
#define INCLUDE_RT_BASE_RT_METADATA_H_

#include <stdint.h>
#include <map>
#include "rt_type.h"   // NOLINT
#include "rt_error.h"  // NOLINT

class RtMetaData;
typedef RT_RET (*RTMetaValueFree)(void *);

struct RTMetaDataContext;

class RtMetaData {
 public:
    RtMetaData();
    RtMetaData(const RtMetaData &from);
    RtMetaData& operator = (const RtMetaData &);

    virtual ~RtMetaData();

    enum Type {
        TYPE_NONE     = MKTAG('n', 'o', 'n', 'e'),
        TYPE_C_STRING = MKTAG('c', 's', 't', 'r'),
        TYPE_INT32    = MKTAG('i', 'n', '3', '2'),
        TYPE_INT64    = MKTAG('i', 'n', '6', '4'),
        TYPE_FLOAT    = MKTAG('f', 'l', 'o', 'a'),
        TYPE_POINTER  = MKTAG('p', 'n', 't', 'r'),
        TYPE_INT8     = MKTAG('i', 'n', '0', '8'),
        TYPE_INT16    = MKTAG('i', 'n', '1', '6'),
        TYPE_STRUCT   = MKTAG('s', 'r', 'u', 't')
    };

    virtual void    clear();
    virtual RT_BOOL remove(UINT64 key);

    virtual RT_BOOL setCString(UINT64 key, const char *value);
    virtual RT_BOOL setInt32(UINT64 key, INT32 value);
    virtual RT_BOOL setInt64(UINT64 key, INT64 value);
    virtual RT_BOOL setFloat(UINT64 key, float value);
    virtual RT_BOOL setPointer(UINT64 key, RT_PTR value, RTMetaValueFree freeFunc = RT_NULL);
    virtual RT_BOOL setStructData(UINT64 key, const void *value, UINT32 size);

    virtual RT_BOOL findCString(UINT64 key, const char **value) const;
    virtual RT_BOOL findInt32(UINT64 key, INT32 *value) const;
    virtual RT_BOOL findInt64(UINT64 key, INT64 *value) const;
    virtual RT_BOOL findFloat(UINT64 key, float *value) const;
    virtual RT_BOOL findPointer(UINT64 key, RT_PTR *value) const;
    virtual RT_BOOL findStructData(UINT64 key, const void **value, UINT32 size) const;

    virtual RT_BOOL setCString(const char* key, const char *value);
    virtual RT_BOOL setInt32(const char* key, INT32 value);
    virtual RT_BOOL setInt64(const char* key, INT64 value);
    virtual RT_BOOL setFloat(const char* key, float value);
    virtual RT_BOOL setPointer(const char* key, RT_PTR value, RTMetaValueFree freeFunc = RT_NULL);

    virtual RT_BOOL findCString(const char* key, const char **value) const;
    virtual RT_BOOL findInt32(const char* key, INT32 *value) const;
    virtual RT_BOOL findInt64(const char* key, INT64 *value) const;
    virtual RT_BOOL findFloat(const char* key, float *value) const;
    virtual RT_BOOL findPointer(const char* key, RT_PTR *value) const;

    virtual RT_BOOL setData(
        UINT64 key, UINT32 type, const void *data, UINT32 size, RTMetaValueFree freeFunc = RT_NULL);

    virtual RT_BOOL findData(UINT64 key, UINT32 *type,
                  const void **data, UINT32 *size) const;

    virtual RT_BOOL hasData(UINT64 key) const;
    virtual RT_BOOL isEmpty();

    virtual void dumpToLog() const;

 private:
    struct              typed_data;
    std::map<UINT64, void *> mDataMaps;
    RTMetaDataContext  *mCtx;
};

extern "C" {

void *createRockitMetaData();
void  destroyRockitMetaData(void **meta);

}

#endif  // INCLUDE_RT_BASE_RT_METADATA_H_

