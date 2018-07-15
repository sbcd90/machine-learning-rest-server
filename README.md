ml_model_rest_server
====================

This project implements a common rest server which can serve tensorflow-serving & xgboost models.

Architecture
============

![Architecture.png](https://github.wdf.sap.corp/raw/I076326/tf_serving_rest_server/master/Architecture.PNG?token=AAAE5wNIgssNkbrlZGR-uOd1qd3GWF2Aks5bR-OFwA%3D%3D)

Installation
============

### Dependencies

- [proxygen](https://github.com/facebook/proxygen)
- [tensorflow](https://github.com/tensorflow/tensorflow)
- [tensorflow-serving](https://github.com/tensorflow/serving)
- [xgboost] (https://github.com/dmlc/xgboost)

### Steps

- ./configure
- cd debug
- make


Example
=======

- To serve tf-serving model, start `tf-serving` first.

```
POST http://localhost:11000

{"input_type": "ints", "inputs": [1, 2, 4, 5], "model_name": "tensorflow-feature-sum-model", "model_signature_name": "tensorflow-feature-sum-model"}
```

- Output

```
{"y_output":"12"}
```

- To serve xgboost models, put the `*.cpb` files in the `debug/src/frontends` directory.

```
POST http://localhost:11000

{"input_type": "floats", "inputs": [1.0, 2.0, 4.0, 5.0], "model_name": "test.cpb", "model_signature_name": "xgboost"}

```

- Output

```
{"y_output":"0.0001"}
```

Sample models
=============

A sample `xgboost test.cpb` model is present in `resources` directory.


Next steps
==========

- docker integration
