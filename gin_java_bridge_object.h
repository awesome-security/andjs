// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDJS_JAVA_GIN_JAVA_BRIDGE_OBJECT_H_
#define ANDJS_JAVA_GIN_JAVA_BRIDGE_OBJECT_H_

#include <map>

#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "gin/handle.h"
#include "gin/interceptor.h"
#include "gin/object_template_builder.h"
#include "gin/wrappable.h"
#include "v8/include/v8-util.h"

#include "base/android/scoped_java_ref.h"
#include "base/memory/ref_counted.h"

#include "content/common/android/gin_java_bridge_errors.h"
#include "content/browser/android/java/gin_java_bound_object.h"

namespace base {
class Value;
class ListValue;
}

namespace andjs {

class GinJavaBridgeObject : public gin::Wrappable<GinJavaBridgeObject>,
                            //public base::RefCountedThreadSafe<GinJavaBridgeObject>,
                            public gin::NamedPropertyInterceptor {
 public:
  static gin::WrapperInfo kWrapperInfo;


  // The following methods can be called on any thread.
  JavaObjectWeakGlobalRef& GetWeakRef() { return ref_; }
  base::android::ScopedJavaLocalRef<jobject> GetLocalRef(JNIEnv* env) {
    return ref_.get(env);
  }

  scoped_refptr<content::GinJavaBoundObject> bound_object() {
    return bound_object_;
  }

  // gin::Wrappable.
  gin::ObjectTemplateBuilder GetObjectTemplateBuilder(
      v8::Isolate* isolate) override;

  // gin::NamedPropertyInterceptor
  v8::Local<v8::Value> GetNamedProperty(v8::Isolate* isolate,
                                        const std::string& property) override;
  std::vector<std::string> EnumerateNamedProperties(
      v8::Isolate* isolate) override;

  const base::android::JavaRef<jclass>& GetSafeAnnotationClass();
  base::android::ScopedJavaLocalRef<jclass> GetLocalClassRef(JNIEnv* env);

  GinJavaBridgeObject(v8::Isolate* isolate, scoped_refptr<content::GinJavaBoundObject> bound_object);

 private:
  ~GinJavaBridgeObject() override;

  v8::Local<v8::FunctionTemplate> GetFunctionTemplate(v8::Isolate* isolate,
                                                      const std::string& name);

  JavaObjectWeakGlobalRef ref_;
  std::map<std::string, bool> known_methods_;

  scoped_refptr<content::GinJavaBoundObject> bound_object_;
  v8::StdGlobalValueMap<std::string, v8::FunctionTemplate> template_cache_;

  DISALLOW_COPY_AND_ASSIGN(GinJavaBridgeObject);
};

}  // namespace andjs

#endif  // ANDJS_JAVA_GIN_JAVA_BRIDGE_OBJECT_H_
