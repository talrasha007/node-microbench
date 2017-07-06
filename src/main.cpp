#include <nnu.h>

using namespace v8;
using namespace node;

void freeNothing(char*, void*) { }

NAN_METHOD(testCall) { }

class EmptyAsync : public Nan::AsyncWorker {
public:
	EmptyAsync(Nan::Callback* cb) : Nan::AsyncWorker(cb) {}
	virtual void Execute() { };
};

NAN_METHOD(testAsync) {
	Nan::Callback *callback = new Nan::Callback(info[0].As<Function>());
	Nan::AsyncQueueWorker(new EmptyAsync(callback));
}

NAN_METHOD(testRetString) {
    info.GetReturnValue().Set(Nan::New("1234567890").ToLocalChecked());
}

NAN_METHOD(testRetArray) {
    Handle<Array> arr = Nan::New<Array>(16);
    for (int i = 0; i < 16; i++) {
        arr->Set(i, Nan::New(i));
    }

    info.GetReturnValue().Set(arr);
}

NAN_METHOD(testGet) {
	Local<Object> obj = info[0]->ToObject();

	int a = obj->Get(Nan::New("a").ToLocalChecked())->Int32Value(),
        b = obj->Get(Nan::New("b").ToLocalChecked())->Int32Value(),
        c = obj->Get(Nan::New("c").ToLocalChecked())->Int32Value();

	info.GetReturnValue().Set(Nan::New<Number>(a + b + c));
}

NAN_METHOD(testArgs) {
    int sum = 0;
    for (int i = 0; i < info.Length(); i++) {
        sum += info[i]->Int32Value();
    }

    info.GetReturnValue().Set(Nan::New<Number>(sum));
}

NAN_METHOD(testArrayGet) {
	Local<Array> arr = Local<Array>::Cast(info[0]->ToObject());
    int sum = 0;
    for (uint32_t i = 0; i < arr->Length(); i++) {
        sum += arr->Get(i)->Int32Value();
    }

	info.GetReturnValue().Set(Nan::New<Number>(sum));
}

NAN_METHOD(testRetObj) {
	Local<Object> obj = Nan::New<Object>();
	obj->Set(Nan::New("a").ToLocalChecked(), Nan::New("1234567890").ToLocalChecked());
	obj->Set(Nan::New("b").ToLocalChecked(), Nan::New("234567890").ToLocalChecked());
	obj->Set(Nan::New("c").ToLocalChecked(), Nan::New("34567890").ToLocalChecked());

	info.GetReturnValue().Set(obj);
}

NAN_METHOD(testBuffer) {
	uint32_t sz = info[0]->Uint32Value();
	info.GetReturnValue().Set(Nan::NewBuffer(sz).ToLocalChecked());
}

NAN_METHOD(testBufferSmalloc) {
	info.GetReturnValue().Set(Nan::NewBuffer(0, 0, freeNothing, 0).ToLocalChecked());
}

NAN_METHOD(testCallback) {
	Nan::Callback(Local<Function>::Cast(info[0])).Call(0, 0);
}

//class IntPtr : public NnuPointer<IntPtr> {
//public:
//	IntPtr() : val_(0) { }
//	~IntPtr() { }
//
//	static void setup(Handle<Object>& exports) {
//		NODE_SET_METHOD(exports, "createIntPtr", createIntPtr);
//		NODE_SET_METHOD(exports, "unwrapIntPtr", unwrap);
//		NODE_SET_METHOD(exports, "valOfPtr", val);
//	}
//
//private:
//	static NAN_METHOD(createIntPtr) {
//		IntPtr *ptr = new IntPtr();
//		info.GetReturnValue().Set(ptr->Wrap());
//	}
//
//	static NAN_METHOD(unwrap) {
//		IntPtr* ptr = Unwrap(args[0]);
//		NanReturnUndefined();
//	}
//
//	static NAN_METHOD(val) {
//		IntPtr* ptr = Unwrap(args[0]);
//
//		if (args[1]->IsNumber()) {
//			ptr->val_ = args[1]->Int32Value();
//			NanReturnUndefined();
//		} else {
//			info.GetReturnValue().Set(Nan::New(ptr->val_));
//		}
//	}
//
//private:
//	int val_;
//};

class TestWrap : public Nan::ObjectWrap {
public:
	static Nan::Persistent<Function> ctor;

	void static setup(Handle<Object> exports) {
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(TestWrap::New);
		tpl->SetClassName(Nan::New("TestWrap").ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		Nan::SetPrototypeMethod(tpl, "unwrap", TestWrap::unwrap);
		Nan::SetPrototypeMethod(tpl, "foo", TestWrap::foo);

		exports->Set(Nan::New("TestWrap").ToLocalChecked(), tpl->GetFunction());
		TestWrap::ctor.Reset(tpl->GetFunction());
	}

	static NAN_METHOD(New) {
		TestWrap* tw = new TestWrap();
		tw->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	static NAN_METHOD(unwrap) {
		TestWrap* tw = Nan::ObjectWrap::Unwrap<TestWrap>(info.This());
		tw->_foo++;
	}

	static NAN_METHOD(foo) { }

private:
	int _foo;
};

Nan::Persistent<Function> TestWrap::ctor;

NAN_METHOD(testWrapNew) {
	info.GetReturnValue().Set(Nan::NewInstance(Nan::New(TestWrap::ctor)).ToLocalChecked());
}

class TestClassWrap : public nnu::ClassWrap<TestClassWrap> {
public:
	static const char * const CLASS_NAME;

	static void setupMember(v8::Local<v8::FunctionTemplate>& tpl) {
		Nan::SetPrototypeMethod(tpl, "unwrap", wrapFunction<&TestClassWrap::unwrap>);
		Nan::SetPrototypeMethod(tpl, "foo", wrapFunction<&TestClassWrap::foo>);
	}

	static NAN_METHOD(ctor) {
		TestClassWrap* cw = new TestClassWrap();
		cw->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(unwrap) {
		_foo++;
	}

	NAN_METHOD(foo) { }

private:
	int _foo;
};

const char * const TestClassWrap::CLASS_NAME = "TestClassWrap";

NAN_METHOD(testClassWrapNew) {
	info.GetReturnValue().Set(TestClassWrap::newInstance());
}

Nan::Persistent<Context> testContext;
Nan::Persistent<Script> testScript;
void initTestContext() {
	Handle<ObjectTemplate> global = Nan::New<ObjectTemplate>();
	global->Set(Nan::New("a").ToLocalChecked(), Nan::New("1234567890").ToLocalChecked());
	global->Set(Nan::New("b").ToLocalChecked(), Nan::New("234567890").ToLocalChecked());
	global->Set(Nan::New("c").ToLocalChecked(), Nan::New("34567890").ToLocalChecked());

	v8::Handle<v8::Context> context = Nan::New<Context>((ExtensionConfiguration*)(NULL), global);
	Context::Scope context_scope(context);
	testContext.Reset(context);

    Handle<Script> script = Nan::CompileScript(Nan::New("({ a: a, b: b, c: c })").ToLocalChecked()).ToLocalChecked();
	testScript.Reset(script);
}

NAN_METHOD(testRun) {
	Context::Scope context_scope(Nan::New(testContext));
	Local<Script> script = Nan::New(testScript);
	info.GetReturnValue().Set(script->Run());
}

NAN_MODULE_INIT(InitAll) {
	initTestContext();

	NAN_EXPORT(target, testRun);

	NAN_EXPORT(target, testCall);
	NAN_EXPORT(target, testAsync);
	NAN_EXPORT(target, testRetString);
    NAN_EXPORT(target, testRetArray);
	NAN_EXPORT(target, testGet);
    NAN_EXPORT(target, testArrayGet);
    NAN_EXPORT(target, testArgs);
    NAN_EXPORT(target, testRetObj);
	NAN_EXPORT(target, testCallback);
	NAN_EXPORT(target, testBuffer);
	NAN_EXPORT(target, testBufferSmalloc);
	NAN_EXPORT(target, testWrapNew);
	NAN_EXPORT(target, testClassWrapNew);
	//IntPtr::setup(exports);
	TestWrap::setup(target);
	TestClassWrap::setup(target);
}

NODE_MODULE(node_microbench, InitAll);
