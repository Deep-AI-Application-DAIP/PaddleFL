## Training/inference in MPC

<img src='../../../images/PFM-design.png' width = "1000" height = "622" align="middle"/>

As in PaddlePaddle, a training or inference job can be separated into the compile-time phase and the run-time phase:

#### Compile time

* **MPC environment specification**: a user needs to choose a MPC protocol, and configure the network settings. In current version, PFM provides federated learning based on PrivC protocol for two-party federated learning and ABY3 protocol for three-party federated learning.

* **User-defined job program**: a user can define the machine learning model structure and the training strategies (or inference task) in a PFM program, using the secure operators.

#### Run time

A PFM program is exactly a PaddlePaddle program, and will be executed as normal PaddlePaddle programs. For example, in run-time a PFM program will be transpiled into ProgramDesc, and then be passed to and run by the Executor. The main concepts in the run-time phase are as follows:

* **Computing nodes**: a computing node is an entity corresponding to a Computing Party. In real deployment, it can be a bare-metal machine, a cloud VM, a docker or even a process. A PFM program will be deployed and run in parallel on all  computing nodes.
* **Operators using MPC**: PFM provides typical machine learning operators in `paddle_fl.mpc` over encrypted data. Such operators are implemented upon PaddlePaddle framework, based on MPC protocols ABY3 and PrivC. Like other PaddlePaddle operators, in run time, instances of PFM operators are created and run in order by Executor.

