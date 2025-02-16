/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY CANON INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CANON INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "JSDOMBinding.h"
#include "JSDOMConvert.h"
#include "JSDOMGuardedObject.h"
#include "JSReadableStream.h"

namespace WebCore {

class ReadableStreamSink;
class ReadableStreamSource;

class ReadableStream final : public DOMGuarded<JSReadableStream> {
public:
    static Ref<ReadableStream> create(JSDOMGlobalObject& globalObject, JSReadableStream& readableStream) { return adoptRef(*new ReadableStream(globalObject, readableStream)); }

    static Ref<ReadableStream> create(JSC::JSGlobalObject&, RefPtr<ReadableStreamSource>&&);

    WEBCORE_EXPORT static bool isDisturbed(JSC::JSGlobalObject&, JSC::JSValue);

    std::pair<Ref<ReadableStream>, Ref<ReadableStream>> tee();

    void lock();
    void pipeTo(ReadableStreamSink&);
    bool isLocked() const;
    bool isDisturbed() const;

    JSReadableStream* readableStream() const { return guarded(); }

private:
    ReadableStream(JSDOMGlobalObject& globalObject, JSReadableStream& readableStream) : DOMGuarded<JSReadableStream>(globalObject, readableStream) { }
};

struct JSReadableStreamWrapperConverter {
    static RefPtr<ReadableStream> toWrapped(JSC::JSGlobalObject& lexicalGlobalObject, JSC::JSValue value)
    {
        JSC::VM& vm = JSC::getVM(&lexicalGlobalObject);
        auto* globalObject = JSC::jsDynamicCast<JSDOMGlobalObject*>(vm, &lexicalGlobalObject);
        if (!globalObject)
            return nullptr;

        auto* readableStream = JSC::jsDynamicCast<JSReadableStream*>(vm, value);
        if (!readableStream)
            return nullptr;

        return ReadableStream::create(*globalObject, *readableStream);
    }
};

template<> struct JSDOMWrapperConverterTraits<ReadableStream> {
    using WrapperClass = JSReadableStreamWrapperConverter;
    using ToWrappedReturnType = RefPtr<ReadableStream>;
    static constexpr bool needsState = true;
};

inline JSC::JSValue toJS(JSC::JSGlobalObject*, JSC::JSGlobalObject*, ReadableStream* stream)
{
    return stream ? stream->readableStream() : JSC::jsUndefined();
}

}
