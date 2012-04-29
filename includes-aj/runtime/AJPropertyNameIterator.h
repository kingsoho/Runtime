
/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
/*
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AJPropertyNameIterator_h
#define AJPropertyNameIterator_h

#include "AJObject.h"
#include "AJString.h"
#include "Operations.h"
#include "PropertyNameArray.h"

namespace AJ {

    class Identifier;
    class AJObject;

    class AJPropertyNameIterator : public AJCell {
        friend class JIT;

    public:
        static AJPropertyNameIterator* create(ExecState*, AJObject*);
        
        static PassRefPtr<Structure> createStructure(AJValue prototype)
        {
            return Structure::create(prototype, TypeInfo(CompoundType, OverridesMarkChildren), AnonymousSlotCount);
        }
        
        virtual ~AJPropertyNameIterator();

        virtual bool isPropertyNameIterator() const { return true; }

        virtual void markChildren(MarkStack&);

        bool getOffset(size_t i, int& offset)
        {
            if (i >= m_numCacheableSlots)
                return false;
            offset = i;
            return true;
        }

        AJValue get(ExecState*, AJObject*, size_t i);
        size_t size() { return m_jsStringsSize; }

        void setCachedStructure(Structure* structure)
        {
            ASSERT(!m_cachedStructure);
            ASSERT(structure);
            m_cachedStructure = structure;
        }
        Structure* cachedStructure() { return m_cachedStructure.get(); }

        void setCachedPrototypeChain(NonNullPassRefPtr<StructureChain> cachedPrototypeChain) { m_cachedPrototypeChain = cachedPrototypeChain; }
        StructureChain* cachedPrototypeChain() { return m_cachedPrototypeChain.get(); }

    private:
        AJPropertyNameIterator(ExecState*, PropertyNameArrayData* propertyNameArrayData, size_t numCacheableSlot);

        RefPtr<Structure> m_cachedStructure;
        RefPtr<StructureChain> m_cachedPrototypeChain;
        uint32_t m_numCacheableSlots;
        uint32_t m_jsStringsSize;
        OwnArrayPtr<AJValue> m_jsStrings;
    };

    inline void Structure::setEnumerationCache(AJPropertyNameIterator* enumerationCache)
    {
        ASSERT(!isDictionary());
        m_enumerationCache = enumerationCache;
    }

    inline void Structure::clearEnumerationCache(AJPropertyNameIterator* enumerationCache)
    {
        m_enumerationCache.clear(enumerationCache);
    }

    inline AJPropertyNameIterator* Structure::enumerationCache()
    {
        return m_enumerationCache.get();
    }

} // namespace AJ

#endif // AJPropertyNameIterator_h
