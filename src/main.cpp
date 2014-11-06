#include <nnu.h>

using namespace v8;
using namespace node;

void freeNothing(char*, void*) { }

NAN_METHOD(testCall) {
	NanScope();
	NanReturnUndefined();
}

NAN_METHOD(testRetString) {
	NanScope();
	NanReturnValue(NanNew("1234567890"));
}

NAN_METHOD(testGet) {
	NanScope();

	Local<Object> obj = args[0]->ToObject();
	int a = obj->Get(NanNew("a"))->Int32Value(),
        b = obj->Get(NanNew("b"))->Int32Value(),
        c = obj->Get(NanNew("c"))->Int32Value();

	NanReturnValue(NanNew<Number>(a + b + c));
}

NAN_METHOD(testRetObj) {
	NanScope();

	Local<Object> obj = NanNew<Object>();
	obj->Set(NanNew("a"), NanNew("1234567890"));
	obj->Set(NanNew("b"), NanNew("234567890"));
	obj->Set(NanNew("c"), NanNew("34567890"));

	NanReturnValue(obj);
}

NAN_METHOD(testBuffer) {
	NanScope();
	NanReturnValue(NanNewBufferHandle(0));
}

NAN_METHOD(testBufferSmalloc) {
	NanScope();
	NanReturnValue(NanNewBufferHandle(0, 0, freeNothing, 0));
}

NAN_METHOD(testCallback) {
	NanScope();
	NanCallback(Local<Function>::Cast(args[0])).Call(0, 0);	
	NanReturnUndefined();
}

class IntPtr : public NnuPointer<IntPtr> {
public:
	IntPtr() : val_(0) { }
	~IntPtr() { }

	static void setup(Handle<Object>& exports) {
		NODE_SET_METHOD(exports, "createIntPtr", createIntPtr);
		NODE_SET_METHOD(exports, "unwrapIntPtr", unwrap);
		NODE_SET_METHOD(exports, "valOfPtr", val);
	}

private:
	static NAN_METHOD(createIntPtr) {
		NanScope();
		IntPtr *ptr = new IntPtr();
		NanReturnValue(ptr->Wrap());
	}

	static NAN_METHOD(unwrap) {
		NanScope();
		IntPtr* ptr = Unwrap(args[0]);
		NanReturnUndefined();
	}

	static NAN_METHOD(val) {
		NanScope();
		IntPtr* ptr = Unwrap(args[0]);

		if (args[1]->IsNumber()) {
			ptr->val_ = args[1]->Int32Value();
			NanReturnUndefined();
		} else {
			NanReturnValue(NanNew(ptr->val_));
		}
	}

private:
	int val_;
};

class TestWrap : public ObjectWrap {
public:
	static Persistent<Function> ctor;

	void static setup(Handle<Object> exports) {
		Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(TestWrap::New);
		tpl->SetClassName(NanNew("TestWrap"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		NODE_SET_PROTOTYPE_METHOD(tpl, "unwrap", TestWrap::unwrap);
		NODE_SET_PROTOTYPE_METHOD(tpl, "foo", TestWrap::foo);

		exports->Set(NanNew("TestWrap"), tpl->GetFunction());
		NanAssignPersistent(TestWrap::ctor, tpl->GetFunction());
	}

	static NAN_METHOD(New) {
		NanScope();

		TestWrap* tw = new TestWrap();
		tw->Wrap(args.This());

		NanReturnValue(args.This());
	}

	static NAN_METHOD(unwrap) {
		NanScope();
		TestWrap* tw = ObjectWrap::Unwrap<TestWrap>(args.This());
		tw->_foo++;
		NanReturnUndefined();
	}

	static NAN_METHOD(foo) {
		NanScope();
		NanReturnUndefined();
	}

private:
	int _foo;
};

Persistent<Function> TestWrap::ctor;

NAN_METHOD(testWrapNew) {
	NanScope();
	NanReturnValue(NanNew(TestWrap::ctor)->NewInstance());
}

Persistent<Context> testContext;
Persistent<Script> testScript;
void initTestContext() {
	Handle<ObjectTemplate> global = NanNew<ObjectTemplate>();
	global->Set(NanNew("a"), NanNew("1234567890"));
	global->Set(NanNew("b"), NanNew("234567890"));
	global->Set(NanNew("c"), NanNew("34567890"));

	v8::Handle<v8::Context> context = NanNewContextHandle(NULL, global);
	Context::Scope context_scope(context);
	NanAssignPersistent(testContext, context);

	Handle<Script> script = Script::Compile(NanNew("({ a: a, b: b, c: c })"));
	NanAssignPersistent(testScript, script);
}

NAN_METHOD(testRun) {
	NanScope();
	//Context::Scope context_scope(NanNew(testContext));
	Local<Script> script = NanNew(testScript);
	NanReturnValue(script->Run());
}

void InitAll(Handle<Object> exports) {
	initTestContext();
	NODE_SET_METHOD(exports, "testRun", testRun);

	NODE_SET_METHOD(exports, "testCall", testCall);
	NODE_SET_METHOD(exports, "testRetString", testRetString);
	NODE_SET_METHOD(exports, "testGet", testGet);
	NODE_SET_METHOD(exports, "testRetObj", testRetObj);
	NODE_SET_METHOD(exports, "testCallback", testCallback);
	NODE_SET_METHOD(exports, "testBuffer", testBuffer);
	NODE_SET_METHOD(exports, "testBufferSmalloc", testBufferSmalloc);
	NODE_SET_METHOD(exports, "testWrapNew", testWrapNew);
	IntPtr::setup(exports);
	TestWrap::setup(exports);
}

NODE_MODULE(node_microbench, InitAll);
