// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "pyopenvino/graph/ops/tensor_iterator.hpp"

#include "openvino/core/node.hpp"
#include "openvino/op/tensor_iterator.hpp"
#include "openvino/op/util/sub_graph_base.hpp"
#include "pyopenvino/graph/ops/util/multisubgraph.hpp"

namespace py = pybind11;

void regclass_graph_op_TensorIterator(py::module m) {
    py::class_<ov::op::v0::TensorIterator, std::shared_ptr<ov::op::v0::TensorIterator>, ov::Node> cls(
        m,
        "tensor_iterator");
    cls.doc() = "openvino.impl.op.TensorIterator wraps ov::op::v0::TensorIterator";
    cls.def(py::init<>());
    cls.def("set_body", &ov::op::v0::TensorIterator::set_body, py::arg("body"));
    cls.def("set_invariant_input",
            &ov::op::v0::TensorIterator::set_invariant_input,
            py::arg("body_parameter"),
            py::arg("value"));
    cls.def("get_iter_value",
            &ov::op::v0::TensorIterator::get_iter_value,
            py::arg("body_value"),
            py::arg("iteration") = -1);
    cls.def("get_num_iterations", &ov::op::v0::TensorIterator::get_num_iterations);

    cls.def("get_concatenated_slices",
            &ov::op::v0::TensorIterator::get_concatenated_slices,
            py::arg("value"),
            py::arg("start"),
            py::arg("stride"),
            py::arg("part_size"),
            py::arg("end"),
            py::arg("axis"));

    cls.def("set_sliced_input",
            &ov::op::v0::TensorIterator::set_sliced_input,
            py::arg("parameter"),
            py::arg("value"),
            py::arg("start"),
            py::arg("stride"),
            py::arg("part_size"),
            py::arg("end"),
            py::arg("axis"));

    cls.def("set_merged_input",
            &ov::op::v0::TensorIterator::set_merged_input,
            py::arg("body_parameter"),
            py::arg("initial_value"),
            py::arg("successive_value"));

    cls.def("get_body", [](const std::shared_ptr<ov::op::v0::TensorIterator>& self) {
        return self->get_body();
    });

    cls.def("get_function", [](const std::shared_ptr<ov::op::v0::TensorIterator>& self) {
        return self->get_function();
    });

    cls.def(
        "set_function",
        [](const std::shared_ptr<ov::op::v0::TensorIterator>& self, const std::shared_ptr<ov::Model>& func) {
            self->set_function(func);
        },
        py::arg("func"));

    cls.def("get_output_descriptions", [](const std::shared_ptr<ov::op::v0::TensorIterator>& self) -> py::list {
        py::list result;

        for (const auto& out_desc : self->get_output_descriptions()) {
            result.append(out_desc);
        }

        return result;
    });

    cls.def("get_input_descriptions", [](const std::shared_ptr<ov::op::v0::TensorIterator>& self) -> py::list {
        py::list result;

        for (const auto& in_desc : self->get_input_descriptions()) {
            result.append(in_desc);
        }

        return result;
    });

    cls.def(
        "set_input_descriptions",
        [](const std::shared_ptr<ov::op::v0::TensorIterator>& self, py::list& inputs) {
            self->set_input_descriptions(0, MultiSubgraphHelpers::list_to_input_descriptor(inputs));
        },
        py::arg("inputs"));

    cls.def(
        "set_output_descriptions",
        [](const std::shared_ptr<ov::op::v0::TensorIterator>& self, py::list outputs) {
            self->set_output_descriptions(0, MultiSubgraphHelpers::list_to_output_descriptor(outputs));
        },
        py::arg("outputs"));
}
