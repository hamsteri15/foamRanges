# FoamRanges
* This libary provides a reference implementation of expression templates for OpenFOAM field arithmetic.

* The purpose here is to demonstrate the performance and implementation of the operations.

## **Compilation**

* You will need a C++17 supporting compiler
* Source the latest OpenFOAM development version and run ./Allwmake


## **Running tests and benchmarks**
* Go to a case folder which has fields p and U in 0 folder and type 
``` Test-Ranges --benchmark-no-analysis ```
* You will see various field operations carried out with the standard OpenFOAM style (old) and with the proposed ranges approach (new)
* If you only wish to run the tests without benchmarks you can type
``` Test-Ranges Test* ```

## **Additional notes**
* The ```unittest/mock_fields.H``` contains "mock" fields which are used to test the ranges implementation without reading mesh
* ```unittest/newGeometricField.H``` and ```unittest/newField.H``` are new field types which allow for direct construction from ranges