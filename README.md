ml_model_rest_server
====================

This project implements a rest server on top of Tensorflow serving written in pure C++.

Architecture
============

![Architecture.png](https://github.wdf.sap.corp/raw/I076326/tf_serving_rest_server/master/Architecture.PNG?token=AAAE5wNIgssNkbrlZGR-uOd1qd3GWF2Aks5bR-OFwA%3D%3D)

Installation
============

- To be documented


Example
=======

```
POST http://localhost:11000

{"input_type": "ints", "inputs": [1, 2, 4, 5], "model_name": "tensorflow-feature-sum-model", "model_signature_name": "tensorflow-feature-sum-model"}

```

- Output

```
{"y_output":"12"}
```

Next steps
==========

- Document the installation steps

- docker integration

- xgboost support
