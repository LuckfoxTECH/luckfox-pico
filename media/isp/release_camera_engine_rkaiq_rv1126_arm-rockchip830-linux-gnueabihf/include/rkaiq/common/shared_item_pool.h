/*
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifndef RKCAM_SHARED_ITEM_POOL_H
#define RKCAM_SHARED_ITEM_POOL_H

#include "safe_list.h"

using namespace XCam;

namespace RkCam {

template<typename T> class SharedItemPool;

template<typename T>
class SharedItemProxy
{
public:
    explicit SharedItemProxy(const SmartPtr<T> &data) : _data(data) {};
    ~SharedItemProxy() {
        if (_pool.ptr())
            _pool->release(_data);
        _data.release();
    };

    void set_buf_pool (SmartPtr<SharedItemPool<T>> pool) {
        _pool = pool;
    }

    SmartPtr<T> &data() {
        return _data;
    }

private:
    XCAM_DEAD_COPY (SharedItemProxy);

private:
    SmartPtr<T>                       _data;
    SmartPtr<SharedItemPool<T>>       _pool;
};

template<typename T>
class SharedItemPool
    : public RefObj
{
    friend class SharedItemProxy<T>;

public:
    explicit SharedItemPool(const char* name, uint32_t max_count = 8);
    virtual ~SharedItemPool();

    SmartPtr<SharedItemProxy<T>> get_item();
    bool has_free_items () {
        return !_item_list.is_empty ();
    }
    uint32_t get_free_item_size () {
        return _item_list.size ();
    }

private:
    SmartPtr<T> allocate_data ();
    void release (SmartPtr<T> &data);
    XCAM_DEAD_COPY (SharedItemPool);

private:
    SafeList<T>              _item_list;
    uint32_t                 _allocated_num;
    uint32_t                 _max_count;
    const char*              _name;
};

};

#include "shared_item_pool.cpp"

#endif //RKCAM_SHARED_ITEM_POOL_H


