/**************************************************************************/
/*  callable_bind_map.cpp                                                 */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "callable_bind_map.h"

const BindArg BindArg::ARG_0 = BindArg(0);
const BindArg BindArg::ARG_1 = BindArg(1);
const BindArg BindArg::ARG_2 = BindArg(2);
const BindArg BindArg::ARG_3 = BindArg(3);
const BindArg BindArg::ARG_4 = BindArg(4);
const BindArg BindArg::ARG_5 = BindArg(5);
const BindArg BindArg::ARG_6 = BindArg(6);
const BindArg BindArg::ARG_7 = BindArg(7);
const BindArg BindArg::ARG_8 = BindArg(8);

uint32_t CallableCustomBindMap::hash() const {
	return callable.hash();
}
String CallableCustomBindMap::get_as_text() const {
	return callable.operator String();
}

bool CallableCustomBindMap::_equal_func(const CallableCustom *p_a, const CallableCustom *p_b) {
	const CallableCustomBindMap *a = static_cast<const CallableCustomBindMap *>(p_a);
	const CallableCustomBindMap *b = static_cast<const CallableCustomBindMap *>(p_b);

	if (a->callable != b->callable) {
		return false;
	}

	if (a->binds.size() != b->binds.size()) {
		return false;
	}

	return true;
}

bool CallableCustomBindMap::_less_func(const CallableCustom *p_a, const CallableCustom *p_b) {
	const CallableCustomBindMap *a = static_cast<const CallableCustomBindMap *>(p_a);
	const CallableCustomBindMap *b = static_cast<const CallableCustomBindMap *>(p_b);

	if (a->callable < b->callable) {
		return true;
	} else if (b->callable < a->callable) {
		return false;
	}

	return a->binds.size() < b->binds.size();
}

CallableCustom::CompareEqualFunc CallableCustomBindMap::get_compare_equal_func() const {
	return _equal_func;
}

CallableCustom::CompareLessFunc CallableCustomBindMap::get_compare_less_func() const {
	return _less_func;
}

bool CallableCustomBindMap::is_valid() const {
	return callable.is_valid();
}

StringName CallableCustomBindMap::get_method() const {
	return callable.get_method();
}

ObjectID CallableCustomBindMap::get_object() const {
	return callable.get_object_id();
}

const Callable *CallableCustomBindMap::get_base_comparator() const {
	return callable.get_base_comparator();
}

void CallableCustomBindMap::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const {
	const Variant **args = (const Variant **)alloca(sizeof(Variant *) * binds.size());
	for (int i = 0; i < binds.size(); i++) {
		if (binds[i]._arg == -1) {
			args[i] = (const Variant *)&binds[i]._custom_bind;
		} else if (binds[i]._arg < p_argcount) {
			args[i] = (const Variant *)p_arguments[binds[i]._arg];
		} else {
			r_call_error.error = Callable::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_call_error.expected = binds[i]._arg;
			return;
		}
	}

	callable.callp(args, p_argcount, r_return_value, r_call_error);
}

Error CallableCustomBindMap::rpc(int p_peer_id, const Variant **p_arguments, int p_argcount, Callable::CallError &r_call_error) const {
	const Variant **args = (const Variant **)alloca(sizeof(Variant *) * binds.size());
	for (int i = 0; i < binds.size(); i++) {
		if (binds[i]._arg == -1) {
			args[i] = (const Variant *)&binds[i]._custom_bind;
		} else if (binds[i]._arg < p_argcount) {
			args[i] = (const Variant *)p_arguments[binds[i]._arg];
		} else {
			r_call_error.error = Callable::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_call_error.expected = binds[i]._arg;
			return ERR_UNCONFIGURED;
		}
	}

	return callable.rpcp(p_peer_id, args, p_argcount, r_call_error);
}

CallableCustomBindMap::CallableCustomBindMap(const Callable &p_callable, const LocalVector<CustomBind> &p_binds) {
	callable = p_callable;
	binds = p_binds;
}

CallableCustomBindMap::~CallableCustomBindMap() {
}
