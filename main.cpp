#include <nan.h>

using namespace v8;
using namespace node;

void freeNothing(char*, void*) { }

NAN_METHOD(testCall) {
	NanScope();
	NanReturnUndefined();
}

NAN_METHOD(testRetString) {
	NanScope();
	NanReturnValue(String::New("1234567890"));
}

NAN_METHOD(testGet) {
	NanScope();

	Local<Object> obj = args[0]->ToObject();
	int a = obj->Get(NanSymbol("a"))->Int32Value(),
        b = obj->Get(NanSymbol("b"))->Int32Value(),
        c = obj->Get(NanSymbol("c"))->Int32Value();

	NanReturnValue(Number::New(a + b + c));
}

NAN_METHOD(testRetObj) {
	NanScope();

	Local<Object> obj = Object::New();
	obj->Set(NanSymbol("a"), String::New("1234567890"));
	obj->Set(NanSymbol("b"), String::New("234567890"));
	obj->Set(NanSymbol("c"), String::New("34567890"));

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

class TestWrap : public ObjectWrap {
public:
	static Persistent<Function> ctor;

	void static setup(Handle<Object> exports) {
		Local<FunctionTemplate> tpl = FunctionTemplate::New(TestWrap::New);
		tpl->SetClassName(String::NewSymbol("TestWrap"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		NODE_SET_METHOD(tpl->PrototypeTemplate(), "unwrap", TestWrap::unwrap);
		NODE_SET_METHOD(tpl->PrototypeTemplate(), "foo", TestWrap::foo);

		exports->Set(String::NewSymbol("TestWrap"), tpl->GetFunction());
		NanAssignPersistent(Function, TestWrap::ctor, tpl->GetFunction());
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
	NanReturnValue(NanPersistentToLocal(TestWrap::ctor)->NewInstance());
}

void InitAll(Handle<Object> exports) {
	NODE_SET_METHOD(exports, "testCall", testCall);
	NODE_SET_METHOD(exports, "testRetString", testRetString);
	NODE_SET_METHOD(exports, "testGet", testGet);
	NODE_SET_METHOD(exports, "testRetObj", testRetObj);
	NODE_SET_METHOD(exports, "testCallback", testCallback);
	NODE_SET_METHOD(exports, "testBuffer", testBuffer);
	NODE_SET_METHOD(exports, "testBufferSmalloc", testBufferSmalloc);
	NODE_SET_METHOD(exports, "testWrapNew", testWrapNew);
	TestWrap::setup(exports);
}

NODE_MODULE(node_microbench, InitAll);
