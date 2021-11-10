/* Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "paddle/fluid/framework/op_registry.h"
#include "mpc_square_op.h"

namespace paddle {
namespace operators {

using Tensor = framework::Tensor;


class MpcSquareOp : public framework::OperatorWithKernel {
public:
    using framework::OperatorWithKernel::OperatorWithKernel;

    void InferShape(framework::InferShapeContext* ctx) const override {
        PADDLE_ENFORCE_EQ(
            ctx->HasInput("X"), true,
            platform::errors::NotFound("Input(X) of MpcSquareOp should not be null."));
        PADDLE_ENFORCE_EQ(
            ctx->HasOutput("Out"), true,
            platform::errors::NotFound("Output(Out) of MpcSquareOp should not be null."));
        ctx->ShareDim("X", /*->*/ "Out");
        ctx->ShareLoD("X", /*->*/ "Out");
    }
};

class MpcSquareOpMaker : public framework::OpProtoAndCheckerMaker {
public:
    void Make() override {
        AddInput("X", "(Tensor), The first input tensor of mpc square op.");
        AddOutput("Out", "(Tensor), The output tensor of mpc square op.");
        AddComment(R"DOC(
MPC square Operator..
)DOC");
    }
};

class MpcSquareGradOp : public framework::OperatorWithKernel {
public:
    using framework::OperatorWithKernel::OperatorWithKernel;
    using Tensor = framework::Tensor;

    void InferShape(framework::InferShapeContext *ctx) const override {
        ctx->ShareDim(framework::GradVarName("Out"), framework::GradVarName("X"));
        ctx->ShareLoD(framework::GradVarName("Out"), framework::GradVarName("X"));
    }
};

template <typename T>
class MpcSquareGradOpMaker : public framework::SingleGradOpMaker<T> {
public:
    using framework::SingleGradOpMaker<T>::SingleGradOpMaker;

protected:
    void Apply(GradOpPtr<T> grad) const override {
        grad->SetType("mpc_square_grad");
        grad->SetInput("X", this->Input("X"));
        grad->SetInput(framework::GradVarName("Out"), this->OutputGrad("Out"));
        grad->SetOutput(framework::GradVarName("X"), this->InputGrad("X"));
    }
};

}  // namespace operators
}  // namespace paddle


namespace ops = paddle::operators;
REGISTER_OPERATOR(mpc_square, ops::MpcSquareOp,
                 ops::MpcSquareOpMaker,
                 ops::MpcSquareGradOpMaker<paddle::framework::OpDesc>);

REGISTER_OPERATOR(mpc_square_grad, ops::MpcSquareGradOp);

#ifdef USE_CUDA

REGISTER_OP_CUDA_KERNEL(
    mpc_square,
    ops::MpcSquareKernel<paddle::platform::CUDADeviceContext, int64_t>);

REGISTER_OP_CUDA_KERNEL(
    mpc_square_grad,
    ops::MpcSquareGradKernel<paddle::platform::CUDADeviceContext, int64_t>);

#else // USE_CUDA

REGISTER_OP_CPU_KERNEL(
    mpc_square,
    ops::MpcSquareKernel<paddle::platform::CPUDeviceContext, int64_t>);

REGISTER_OP_CPU_KERNEL(
    mpc_square_grad,
    ops::MpcSquareGradKernel<paddle::platform::CPUDeviceContext, int64_t>);

#endif // USE_CUDA
