const std = @import("std");
pub usingnamespace @import("std").c.builtins;

const Template = opaque {};
const Isolate = opaque {};
const Platform = opaque {};
const Value = opaque {};
const Context = opaque {};
const CallbackInfo = opaque {};

pub extern fn v8_version(...) [*c]const u8;
pub extern fn v8_create_function_template(*Isolate) *Template;

pub extern fn v8_isolate_get_current(...) *Isolate;
pub extern fn v8_new_isolate(...) *Isolate;

pub extern fn v8_initialize(...) void;
pub extern fn v8_initialize_platform(*Platform) void;
pub extern fn v8_dispose(...) void;
pub extern fn v8_shutdown_platform(...) void;
pub extern fn v8_new_default_platform(...) *Platform;
pub extern fn v8_run_script(*Isolate, [*c]const u8) *Value;

const Callback = fn (*CallbackInfo, c_int) callconv (.C) void;
pub extern fn v8_create_function(*Isolate, Callback) *Value;

fn add(cb_info: *CallbackInfo, len: c_int) callconv(.C) void {
    return;
}

pub fn main() anyerror!void {
    const version = v8_version();
    std.log.info("V8 Version: {s}", .{ version });
    v8_initialize();
    const platform = v8_new_default_platform();
    v8_initialize_platform(platform);

    const isolate = v8_new_isolate();
   

    {

        // const isolate = v8_isolate_get_current();
        // const tmp = v8_create_function_template(isolate);
        const value = v8_run_script(isolate, "1 + 1");
         const add_fn = v8_create_function(isolate, add);
    }
    v8_dispose();
    v8_shutdown_platform();
}

