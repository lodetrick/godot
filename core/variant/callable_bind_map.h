/**************************************************************************/
/*  callable_bind_map.h                                                   */
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

#pragma once

#include "callable_bind.h"
#include "core/io/resource.h"
#include "core/variant/callable.h"
#include "core/variant/variant.h"

// This is a class in order to perform type checks against it.
class BindArg : public Object {
	GDCLASS(BindArg, Object);

public:
	static const BindArg ARG_0;
	static const BindArg ARG_1;
	static const BindArg ARG_2;
	static const BindArg ARG_3;
	static const BindArg ARG_4;
	static const BindArg ARG_5;
	static const BindArg ARG_6;
	static const BindArg ARG_7;
	static const BindArg ARG_8;

private:
	int _arg = -1;

	BindArg(int p_pos) :
			_arg(p_pos) {}

public:
	inline int get_arg() const { return _arg; }
};

class CallableCustomBindMap : public CallableCustom {
public:
	struct CustomBind {
		Variant _custom_bind;
		int _arg = -1;

		CustomBind() {}
		CustomBind(Variant p_custom_bind) :
				_custom_bind(p_custom_bind) {}
		CustomBind(BindArg p_arg) :
				_arg(p_arg.get_arg()) {}
	};

private:
	Callable callable;
	LocalVector<CustomBind> binds;

	static bool _equal_func(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool _less_func(const CallableCustom *p_a, const CallableCustom *p_b);

public:
	//for every type that inherits, these must always be the same for this type
	virtual uint32_t hash() const override;
	virtual String get_as_text() const override;
	virtual CompareEqualFunc get_compare_equal_func() const override;
	virtual CompareLessFunc get_compare_less_func() const override;
	virtual bool is_valid() const override;
	virtual StringName get_method() const override;
	virtual ObjectID get_object() const override;
	virtual void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;
	virtual Error rpc(int p_peer_id, const Variant **p_arguments, int p_argcount, Callable::CallError &r_call_error) const override;
	virtual const Callable *get_base_comparator() const override;

	Callable get_callable() { return callable; }

	CallableCustomBindMap(const Callable &p_callable, const LocalVector<CustomBind> &p_binds);
	virtual ~CallableCustomBindMap();
};
