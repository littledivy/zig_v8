#include "v8.h"
#include "libplatform/libplatform.h"

extern "C" {
const char* v8_version() { 
    return v8::V8::GetVersion(); 
}

void v8_initialize_platform(v8::Platform* platform) {
  v8::V8::InitializePlatform(platform);
}

v8::Platform* v8_new_default_platform() {
  return v8::platform::NewDefaultPlatform().release();
}

void v8_initialize() { v8::V8::Initialize(); }

bool v8_dispose() { return v8::V8::Dispose(); }

void v8_shutdown_platform() { v8::V8::ShutdownPlatform(); }

v8::Isolate* v8_isolate_get_current() {
    return v8::Isolate::GetCurrent();
}

v8::Template* v8_create_function_template(v8::Isolate* isolate) {
    v8::Local<v8::Template> local = v8::FunctionTemplate::New(isolate);
    return *local;
}

// TODO: make this configurable
v8::Isolate* v8_new_isolate() {
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    create_params.only_terminate_in_safe_scope = true;

    v8::Isolate *isolate = v8::Isolate::New(create_params);

    isolate->SetCaptureStackTraceForUncaughtExceptions(true);
    return isolate;
}

v8::Local<v8::Value> v8_run_script(v8::Isolate* isolate, const char* source_code) {
    v8::Isolate::Scope isolate_scope(isolate);
     v8::HandleScope handle_scope(isolate);   
    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);
    
    v8::MaybeLocal<v8::String> maybe_source = v8::String::NewFromUtf8(isolate, source_code);

    v8::MaybeLocal<v8::Script> maybe_script = v8::Script::Compile(context, maybe_source.ToLocalChecked());

    v8::Local<v8::Value> result_def = static_cast<v8::Local<v8::Value>>(v8::Undefined(isolate));
    return maybe_script.ToLocalChecked()->Run(context).FromMaybe(result_def);
}
 
typedef void (*v8_func_cb_t)(v8::FunctionCallbackInfo<v8::Value> cb_info, int argc);


void v8_callback_function(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8_func_cb_t func_cb          =  *reinterpret_cast<v8_func_cb_t*>(v8::Local<v8::External>::Cast(args.Data())->Value());

    func_cb(args, args.Length());
}

v8::Local<v8::Value> v8_create_function(v8::Isolate* isolate, v8_func_cb_t cb)
{
;
     v8::HandleScope handle_scope(isolate);   
     v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);
    v8::Local<v8::Value> cb_data = v8::External::New(isolate, reinterpret_cast<void*>(cb));

    return (
            v8::Function::New(
                context,
                v8_callback_function,
                cb_data
            ).ToLocalChecked()
    );
}
} // extern "C"