/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#include <af/array.h>
#include <af/dim4.hpp>
#include <af/internal.h>
#include <af/traits.hpp>
#include <arrayfire.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cfloat>
#include <fstream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

typedef unsigned char  uchar;
typedef unsigned int   uint;
typedef unsigned short ushort;

std::string readNextNonEmptyLine(std::ifstream &file)
{
    std::string result = "";
    // Using a for loop to read the next non empty line
    for (std::string line; std::getline(file, line);) {
        result += line;
        if (result != "") break;
    }
    // If no file has been found, throw an exception
    if (result == "") {
        throw std::runtime_error("Non empty lines not found in the file");
    }
    return result;
}

template<typename inType, typename outType, typename FileElementType>
void readTests(const std::string &FileName, std::vector<af::dim4> &inputDims,
                std::vector<std::vector<inType> >   &testInputs,
                std::vector<std::vector<outType> > &testOutputs)
{
    using std::vector;

    std::ifstream testFile(FileName.c_str());
    if(testFile.good()) {
        unsigned inputCount;
        testFile >> inputCount;
        inputDims.resize(inputCount);
        for(unsigned i=0; i<inputCount; i++) {
            testFile >> inputDims[i];
        }

        unsigned testCount;
        testFile >> testCount;
        testOutputs.resize(testCount);

        vector<unsigned> testSizes(testCount);
        for(unsigned i = 0; i < testCount; i++) {
            testFile >> testSizes[i];
        }

        testInputs.resize(inputCount,vector<inType>(0));
        for(unsigned k=0; k<inputCount; k++) {
            unsigned nElems = inputDims[k].elements();
            testInputs[k].resize(nElems);
            FileElementType tmp;
            for(unsigned i = 0; i < nElems; i++) {
                testFile >> tmp;
                testInputs[k][i] = static_cast<inType>(tmp);
            }
        }

        testOutputs.resize(testCount, vector<outType>(0));
        for(unsigned i = 0; i < testCount; i++) {
            testOutputs[i].resize(testSizes[i]);
            FileElementType tmp;
            for(unsigned j = 0; j < testSizes[i]; j++) {
                testFile >> tmp;
                testOutputs[i][j] = static_cast<outType>(tmp);
            }
        }
    }
    else {
        FAIL() << "TEST FILE NOT FOUND";
    }
}

template<typename inType, typename outType>
void readTestsFromFile(const std::string &FileName, std::vector<af::dim4> &inputDims,
                std::vector<std::vector<inType> >  &testInputs,
                std::vector<std::vector<outType> > &testOutputs)
{
    using std::vector;

    std::ifstream testFile(FileName.c_str());
    if(testFile.good()) {
        unsigned inputCount;
        testFile >> inputCount;
        for(unsigned i=0; i<inputCount; i++) {
            af::dim4 temp(1);
            testFile >> temp;
            inputDims.push_back(temp);
        }

        unsigned testCount;
        testFile >> testCount;
        testOutputs.resize(testCount);

        vector<unsigned> testSizes(testCount);
        for(unsigned i = 0; i < testCount; i++) {
            testFile >> testSizes[i];
        }

        testInputs.resize(inputCount,vector<inType>(0));
        for(unsigned k=0; k<inputCount; k++) {
            unsigned nElems = inputDims[k].elements();
            testInputs[k].resize(nElems);
            inType tmp;
            for(unsigned i = 0; i < nElems; i++) {
                testFile >> tmp;
                testInputs[k][i] = tmp;
            }
        }

        testOutputs.resize(testCount, vector<outType>(0));
        for(unsigned i = 0; i < testCount; i++) {
            testOutputs[i].resize(testSizes[i]);
            outType tmp;
            for(unsigned j = 0; j < testSizes[i]; j++) {
                testFile >> tmp;
                testOutputs[i][j] = tmp;
            }
        }
    }
    else {
        FAIL() << "TEST FILE NOT FOUND";
    }
}

inline void readImageTests(const std::string        &pFileName,
                           std::vector<af::dim4>    &pInputDims,
                           std::vector<std::string> &pTestInputs,
                           std::vector<dim_t>    &pTestOutSizes,
                           std::vector<std::string> &pTestOutputs)
{
    using std::vector;

    std::ifstream testFile(pFileName.c_str());
    if(testFile.good()) {
        unsigned inputCount;
        testFile >> inputCount;
        for(unsigned i=0; i<inputCount; i++) {
            af::dim4 temp(1);
            testFile >> temp;
            pInputDims.push_back(temp);
        }

        unsigned testCount;
        testFile >> testCount;
        pTestOutputs.resize(testCount);

        pTestOutSizes.resize(testCount);
        for(unsigned i = 0; i < testCount; i++) {
            testFile >> pTestOutSizes[i];
        }

        pTestInputs.resize(inputCount, "");
        for(unsigned k=0; k<inputCount; k++) {
            pTestInputs[k] = readNextNonEmptyLine(testFile);
        }

        pTestOutputs.resize(testCount, "");
        for(unsigned i = 0; i < testCount; i++) {
            pTestOutputs[i] = readNextNonEmptyLine(testFile);
        }
    }
    else {
        FAIL() << "TEST FILE NOT FOUND";
    }
}

template<typename outType>
void readImageTests(const std::string                 &pFileName,
                    std::vector<af::dim4>             &pInputDims,
                    std::vector<std::string>          &pTestInputs,
                    std::vector<std::vector<outType> > &pTestOutputs)
{
    using std::vector;

    std::ifstream testFile(pFileName.c_str());
    if(testFile.good()) {
        unsigned inputCount;
        testFile >> inputCount;
        for(unsigned i=0; i<inputCount; i++) {
            af::dim4 temp(1);
            testFile >> temp;
            pInputDims.push_back(temp);
        }

        unsigned testCount;
        testFile >> testCount;
        pTestOutputs.resize(testCount);

        vector<unsigned> testSizes(testCount);
        for(unsigned i = 0; i < testCount; i++) {
            testFile >> testSizes[i];
        }

        pTestInputs.resize(inputCount, "");
        for(unsigned k=0; k<inputCount; k++) {
            pTestInputs[k] = readNextNonEmptyLine(testFile);
        }

        pTestOutputs.resize(testCount, vector<outType>(0));
        for(unsigned i = 0; i < testCount; i++) {
            pTestOutputs[i].resize(testSizes[i]);
            outType tmp;
            for(unsigned j = 0; j < testSizes[i]; j++) {
                testFile >> tmp;
                pTestOutputs[i][j] = tmp;
            }
        }
    }
    else {
        FAIL() << "TEST FILE NOT FOUND";
    }
}

template<typename descType>
void readImageFeaturesDescriptors(const std::string                  &pFileName,
                                  std::vector<af::dim4>              &pInputDims,
                                  std::vector<std::string>           &pTestInputs,
                                  std::vector<std::vector<float> >    &pTestFeats,
                                  std::vector<std::vector<descType> > &pTestDescs)
{
    using std::vector;

    std::ifstream testFile(pFileName.c_str());
    if(testFile.good()) {
        unsigned inputCount;
        testFile >> inputCount;
        for(unsigned i=0; i<inputCount; i++) {
            af::dim4 temp(1);
            testFile >> temp;
            pInputDims.push_back(temp);
        }

        unsigned attrCount, featCount, descLen;
        testFile >> featCount;
        testFile >> attrCount;
        testFile >> descLen;
        pTestFeats.resize(attrCount);

        pTestInputs.resize(inputCount, "");
        for(unsigned k=0; k<inputCount; k++) {
            pTestInputs[k] = readNextNonEmptyLine(testFile);
        }

        pTestFeats.resize(attrCount, vector<float>(0));
        for(unsigned i = 0; i < attrCount; i++) {
            pTestFeats[i].resize(featCount);
            float tmp;
            for(unsigned j = 0; j < featCount; j++) {
                testFile >> tmp;
                pTestFeats[i][j] = tmp;
            }
        }

        pTestDescs.resize(featCount, vector<descType>(0));
        for(unsigned i = 0; i < featCount; i++) {
            pTestDescs[i].resize(descLen);
            descType tmp;
            for(unsigned j = 0; j < descLen; j++) {
                testFile >> tmp;
                pTestDescs[i][j] = tmp;
            }
        }
    }
    else {
        FAIL() << "TEST FILE NOT FOUND";
    }
}

/**
 * Below is not a pair wise comparition method, rather
 * it computes the accumulated error of the computed
 * output and gold output.
 *
 * The cut off is decided based on root mean square
 * deviation from cpu result
 *
 * For images, the maximum possible error will happen if all
 * the observed values are zeros and all the predicted values
 * are 255's. In such case, the value of NRMSD will be 1.0
 * Similarly, we can deduce that 0.0 will be the minimum
 * value of NRMSD. Hence, the range of RMSD is [0,255] for image inputs.
 */
template<typename T>
bool compareArraysRMSD(dim_t data_size, T *gold, T *data, double tolerance)
{
    double accum  = 0.0;
    double maxion = -FLT_MAX;//(double)std::numeric_limits<T>::lowest();
    double minion = FLT_MAX;//(double)std::numeric_limits<T>::max();

    for(dim_t i=0;i<data_size;i++)
    {
        double dTemp = (double)data[i];
        double gTemp = (double)gold[i];
        double diff  = gTemp-dTemp;
        double err   = (std::isfinite(diff) && (std::abs(diff) > 1.0e-4)) ? diff : 0.0f;
        accum  += std::pow(err, 2.0);
        maxion  = std::max(maxion, dTemp);
        minion  = std::min(minion, dTemp);
    }
    accum /= data_size;
    double NRMSD = std::sqrt(accum)/(maxion-minion);

    if (std::isnan(NRMSD) || NRMSD > tolerance) {
#ifndef NDEBUG
        printf("Comparison failed, NRMSD value: %lf\n", NRMSD);
#endif
        return false;
    }

    return true;
}

template<typename T, typename Other>
struct is_same_type {
    static const bool value = false;
};

template<typename T>
struct is_same_type<T, T> {
    static const bool value = true;
};

template<bool, typename T, typename O>
struct cond_type;

template<typename T, typename Other>
struct cond_type<true, T, Other> {
    typedef T type;
};

template<typename T, typename Other>
struct cond_type<false, T, Other> {
    typedef Other type;
};

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template<typename T>
inline double real(T val) { return (double)val; }
template<>
inline double real<af::cdouble>(af::cdouble val) { return real(val); }
template<>
inline double real<af::cfloat> (af::cfloat val) { return real(val); }

template<typename T>
inline double imag(T val) { return (double)val; }
template<>
inline double imag<af::cdouble>(af::cdouble val) { return imag(val); }
template<>
inline double imag<af::cfloat> (af::cfloat val) { return imag(val); }


template<class T>
struct IsFloatingPoint {
    static const bool value = is_same_type<float, T>::value  ||
        is_same_type<double, T>::value  ||
        is_same_type<long double, T>::value;
};

template<typename T>
bool noDoubleTests()
{
    af::dtype ty = (af::dtype)af::dtype_traits<T>::af_type;
    bool isTypeDouble = (ty == f64) || (ty == c64);
    int dev = af::getDevice();
    bool isDoubleSupported = af::isDoubleAvailable(dev);

    return ((isTypeDouble && !isDoubleSupported) ? true : false);
}

inline bool noImageIOTests()
{
    bool ret = !af::isImageIOAvailable();
    if(ret) printf("Image IO Not Configured. Test will exit\n");
    return ret;
}

inline bool noLAPACKTests()
{
    bool ret = !af::isLAPACKAvailable();
    if(ret) printf("LAPACK Not Configured. Test will exit\n");
    return ret;
}

// TODO: perform conversion on device for CUDA and OpenCL
template<typename T>
af_err conv_image(af_array *out, af_array in)
{
    af_array outArray;

    dim_t d0, d1, d2, d3;
    af_get_dims(&d0, &d1, &d2, &d3, in);
    af::dim4 idims(d0, d1, d2, d3);

    dim_t nElems = 0;
    af_get_elements(&nElems, in);

    float *in_data = new float[nElems];
    af_get_data_ptr(in_data, in);

    T *out_data = new T[nElems];

    for (int i = 0; i < (int)nElems; i++)
        out_data[i] = (T)in_data[i];

    af_create_array(&outArray, out_data, idims.ndims(), idims.get(), (af_dtype) af::dtype_traits<T>::af_type);

    std::swap(*out, outArray);

    delete [] in_data;
    delete [] out_data;

    return AF_SUCCESS;
}

template<typename T>
af::array cpu_randu(const af::dim4 dims)
{
    typedef typename af::dtype_traits<T>::base_type BT;

    bool isTypeCplx = is_same_type<T, af::cfloat>::value || is_same_type<T, af::cdouble>::value;
    bool isTypeFloat = is_same_type<BT, float>::value || is_same_type<BT, double>::value;

    dim_t elements = (isTypeCplx ? 2 : 1) * dims.elements();

    std::vector<BT> out(elements);
    for(int i = 0; i < (int)elements; i++) {
        out[i] = isTypeFloat ? (BT)(rand())/RAND_MAX : rand() % 100;
    }

    return af::array(dims, (T *)&out[0]);
}

void cleanSlate()
{
  const size_t step_bytes = 1024;

  size_t alloc_bytes, alloc_buffers;
  size_t lock_bytes, lock_buffers;

  af::deviceGC();

  af::deviceMemInfo(&alloc_bytes, &alloc_buffers,
                    &lock_bytes, &lock_buffers);

  ASSERT_EQ(0u, alloc_buffers);
  ASSERT_EQ(0u, lock_buffers);
  ASSERT_EQ(0u, alloc_bytes);
  ASSERT_EQ(0u, lock_bytes);

  af::setMemStepSize(step_bytes);

  ASSERT_EQ(af::getMemStepSize(), step_bytes);
}

std::ostream& operator<<(std::ostream& os, af_err e) {
    return os << af_err_to_string(e);
}

std::ostream& operator<<(std::ostream& os, af::dtype type) {
    std::string name;
    switch (type) {
    case f32: name = "f32"; break;
    case c32: name = "c32"; break;
    case f64: name = "f64"; break;
    case c64: name = "c64"; break;
    case b8:  name = "b8";  break;
    case s32: name = "s32"; break;
    case u32: name = "u32"; break;
    case u8:  name = "u8";  break;
    case s64: name = "s64"; break;
    case u64: name = "u64"; break;
    case s16: name = "s16"; break;
    case u16: name = "u16"; break;
    default: assert(false && "Invalid type");
    }
    return os << name;
}

// Calculate a linearized index's multi-dimensonal coordinates in an af::array,
//  given its dimension sizes and strides
af::dim4 unravelIdx(uint idx, af::dim4 dims, af::dim4 strides) {
    af::dim4 coords;
    coords[3] = idx / (strides[3]);
    coords[2] = idx / (strides[2]) % dims[2];
    coords[1] = idx / (strides[1]) % dims[1];
    coords[0] = idx % dims[0];

    return coords;
}

af::dim4 unravelIdx(uint idx, af::array arr) {
    af::dim4 dims = arr.dims();
    af::dim4 st = af::getStrides(arr);
    return unravelIdx(idx, dims, st);
}

struct FloatTag {};
struct IntegerTag {};

af::dim4 calcStrides(const af::dim4 &parentDim)
{
    af::dim4 out(1, 1, 1, 1);
    dim_t *out_dims = out.get();
    const dim_t *parent_dims =  parentDim.get();

    for (dim_t i=1; i < 4; i++) {
        out_dims[i] = out_dims[i - 1] * parent_dims[i-1];
    }

    return out;
}

template<typename T>
::testing::AssertionResult elemWiseEq(std::string aName, std::string bName,
                                      std::vector<T>& a, af::dim4 aDims,
                                      std::vector<T>& b, af::dim4 bDims,
                                      float maxAbsDiff, IntegerTag) {
    typedef typename std::vector<T>::iterator iter;
    std::pair<iter, iter> mismatches = std::mismatch(a.begin(), a.end(), b.begin());
    iter aItr = mismatches.first;
    iter bItr = mismatches.second;

    if (bItr == b.end()) {
        return ::testing::AssertionSuccess();
    } else {
        int idx = std::distance(b.begin(), bItr);
        af::dim4 bStrides = calcStrides(bDims);
        af::dim4 coords = unravelIdx(idx, bDims, bStrides);

        return ::testing::AssertionFailure() << "VALUE DIFFERS:\n"
                                             << " at ([" << coords << "]):\n"
                                             << aName << "(" << a[idx] << ")\n"
                                             << bName << "(" << b[idx] << ")";
    }
}

template<typename T>
::testing::AssertionResult elemWiseEq(std::string aName, std::string bName,
                                      std::vector<T>& a, af::dim4 aDims,
                                      std::vector<T>& b, af::dim4 bDims,
                                      float maxAbsDiff, FloatTag) {
    typedef typename std::vector<T>::iterator iter;
    // TODO(mark): Modify equality for float
    std::pair<iter, iter> mismatches = std::mismatch(a.begin(), a.end(), b.begin());
    iter aItr = mismatches.first;
    iter bItr = mismatches.second;

    // ASSERT_NEAR
    if (maxAbsDiff != 0) {
        using std::abs;
        using af::abs;
        for (int idx = 0; idx < a.size(); ++idx) {
            double absdiff = abs(a[idx] - b[idx]);
            if (absdiff > maxAbsDiff) {
                af::dim4 coords = unravelIdx(idx, aDims, calcStrides(aDims));
                return ::testing::AssertionFailure() << "ABS DIFF EXCEEDS THRESHOLD:\n"
                                                     << " at ([" << coords << "]):\n"
                                                     << aName << "(" << a[idx] << ")\n"
                                                     << bName << "(" << b[idx] << ")\n"
                                                     << "Expected abs diff: " << maxAbsDiff << "\n"
                                                     << "Actual abs diff  : " << absdiff;
            }
        }
        return ::testing::AssertionSuccess();
    }
    // ASSERT_EQ
    else {
        if (bItr == b.end()) {
            return ::testing::AssertionSuccess();
        } else {
            int idx = std::distance(b.begin(), bItr);
            af::dim4 coords = unravelIdx(idx, bDims, calcStrides(bDims));

            return ::testing::AssertionFailure() << "VALUE DIFFERS:\n"
                                                 << " at ([" << coords << "]):\n"
                                                 << aName << "(" << a[idx] << ")\n"
                                                 << bName << "(" << b[idx] << ")";
        }
    }
}

template<typename T>
::testing::AssertionResult elemWiseEq(std::string aName, std::string bName,
                                      af::array a, af::array b, float maxAbsDiff) {
    typedef typename cond_type<
        IsFloatingPoint<typename af::dtype_traits<T>::base_type>::value,
        FloatTag, IntegerTag>::type TagType;
    TagType tag;

    std::vector<T> hA(a.elements());
    a.host(hA.data());

    std::vector<T> hB(b.elements());
    b.host(hB.data());
    return elemWiseEq<T>(aName, bName, hA, a.dims(), hB, b.dims(), maxAbsDiff, tag);
}

::testing::AssertionResult assertArrayEq(std::string aName, std::string bName,
                                         af::array a, af::array b,
                                         float maxAbsDiff = 0.f) {
    af::dtype aType = a.type();
    af::dtype bType = b.type();
    if (aType != bType)
        return ::testing::AssertionFailure() << "TYPE MISMATCH: \n"
                                             << "Expected: " << aName << "(" << a.type() << ")\n"
                                             << "Actual:   " << bName << "(" << b.type() << ")";
    af::dtype arrDtype = aType;
    const uint ndimIds = 4;
    if (a.dims() != b.dims())
        return ::testing::AssertionFailure() << "SIZE MISMATCH: \n"
                                             << "Expected: " << aName << "([" << a.dims() << "]),\n"
                                             << "Actual:   " << bName << "([" << b.dims() << "])";

    uint nElems = a.elements();
    af::dim4 arrDims = a.dims();

    switch (arrDtype) {
    case f32: return elemWiseEq<float>              (aName, bName, a, b, maxAbsDiff); break;
    case c32: return elemWiseEq<af::cfloat>         (aName, bName, a, b, maxAbsDiff); break;
    case f64: return elemWiseEq<double>             (aName, bName, a, b, maxAbsDiff); break;
    case c64: return elemWiseEq<af::cdouble>        (aName, bName, a, b, maxAbsDiff); break;
    case b8:  return elemWiseEq<char>               (aName, bName, a, b, maxAbsDiff); break;
    case s32: return elemWiseEq<int>                (aName, bName, a, b, maxAbsDiff); break;
    case u32: return elemWiseEq<uint>               (aName, bName, a, b, maxAbsDiff); break;
    case u8:  return elemWiseEq<uchar>              (aName, bName, a, b, maxAbsDiff); break;
    case s64: return elemWiseEq<long long>          (aName, bName, a, b, maxAbsDiff); break;
    case u64: return elemWiseEq<unsigned long long> (aName, bName, a, b, maxAbsDiff); break;
    case s16: return elemWiseEq<short>              (aName, bName, a, b, maxAbsDiff); break;
    case u16: return elemWiseEq<unsigned short>     (aName, bName, a, b, maxAbsDiff); break;
    default:  return ::testing::AssertionFailure()
            << "INVALID TYPE, see enum numbers: "
            << aName << "(" << a.type() << ") and "
            << bName << "(" << b.type() << ")";
    }

    return ::testing::AssertionSuccess();
}

template<typename T>
::testing::AssertionResult assertArrayEq(std::string aName, std::string aDimsName,
                                         std::string bName,
                                         std::vector<T>& hA, af::dim4 aDims,
                                         af::array b,
                                         float maxAbsDiff = 0.0f) {
    af::dtype aDtype = (af::dtype) af::dtype_traits<T>::af_type;
    if (aDtype != b.type()) {
        return ::testing::AssertionFailure() << "TYPE MISMATCH:\n"
                                             << "Expected: " << aName << "(" << aDtype << ")\n"
                                             << "Actual:   " << bName << "(" << b.type() << ")";
    }

    const uint ndimIds = 4;
    if(aDims != b.dims()) {
        return ::testing::AssertionFailure() << "SIZE MISMATCH:\n"
                                             << "Expected: " << aDimsName << "([" << aDims << "])\n"
                                             << "Actual:   " << bName << "([" << b.dims() << "])";
    }

    // In case vector<T> a.size() != aDims.elements()
    if (hA.size() != aDims.elements())
        return ::testing::AssertionFailure() << "SIZE MISMATCH:\n"
                                             << "Expected: " << aName << ".size()(" << hA.size() << ")\n"
                                             << "Actual:   " << aDimsName << "([" << aDims << "] => "
                                             << aDims.elements() << ")";

    typedef typename cond_type<
        IsFloatingPoint<typename af::dtype_traits<T>::base_type>::value,
        FloatTag, IntegerTag>::type TagType;
    TagType tag;

    std::vector<T> hB(b.elements());
    b.host(&hB.front());
    return elemWiseEq<T>(aName, bName, hA, aDims, hB, b.dims(), maxAbsDiff, tag);
}

// To support C API
::testing::AssertionResult assertArrayEq(std::string aName, std::string bName,
                                         af_array a, af_array b) {
    af_array aa = 0, bb = 0;
    af_retain_array(&aa, a);
    af_retain_array(&bb, b);
    af::array aaa(aa);
    af::array bbb(bb);
    return assertArrayEq(aName, bName, aaa, bbb, 0.0f);
}

// To support C API
template<typename T>
::testing::AssertionResult assertArrayEq(std::string hA_name, std::string aDimsName,
                                         std::string bName,
                                         std::vector<T>& hA, af::dim4 aDims, af_array b) {
    af_array bb = 0;
    af_retain_array(&bb, b);
    af::array bbb(bb);
    return assertArrayEq(hA_name, aDimsName, bName, hA, aDims, bbb);
}

::testing::AssertionResult assertArrayNear(std::string aName, std::string bName,
                                           std::string maxAbsDiffName,
                                           af::array a, af::array b, float maxAbsDiff) {
    return assertArrayEq(aName, bName, a, b, maxAbsDiff);
}

template<typename T>
::testing::AssertionResult assertArrayNear(std::string hA_name, std::string aDimsName,
                                           std::string bName,
                                           std::string maxAbsDiffName,
                                           std::vector<T>& hA, af::dim4 aDims,
                                           af::array b,
                                           float maxAbsDiff) {
    return assertArrayEq(hA_name, aDimsName, bName, hA, aDims, b, maxAbsDiff);
}

// To support C API
::testing::AssertionResult assertArrayNear(std::string aName, std::string bName,
                                           std::string maxAbsDiffName,
                                           af_array a, af_array b, float maxAbsDiff) {
    af_array aa = 0, bb = 0;
    af_retain_array(&aa, a);
    af_retain_array(&bb, b);
    af::array aaa(aa);
    af::array bbb(bb);
    return assertArrayNear(aName, bName, maxAbsDiffName, aaa, bbb, maxAbsDiff);
}

// To support C API
template<typename T>
::testing::AssertionResult assertArrayNear(std::string hA_name, std::string aDimsName,
                                           std::string bName,
                                           std::string maxAbsDiffName,
                                           std::vector<T>& hA, af::dim4 aDims,
                                           af_array b,
                                           float maxAbsDiff) {
    af_array bb = 0;
    af_retain_array(&bb, b);
    af::array bbb(bb);
    return assertArrayNear(hA_name, aDimsName, maxAbsDiffName, bName, hA, aDims,
                         bbb, maxAbsDiff);
}

/// Checks if the C-API arrayfire function returns successfully
///
/// \param[in] CALL This is the arrayfire C function
#define ASSERT_SUCCESS(CALL)                    \
    ASSERT_EQ(AF_SUCCESS, CALL)

/// Compares two af::array or af_arrays for their types, dims, and values (strict equality).
///
/// \param[in] EXPECTED The expected array of the assertion
/// \param[in] ACTUAL The actual resulting array from the calculation
#define ASSERT_ARRAYS_EQ(EXPECTED, ACTUAL) \
    EXPECT_PRED_FORMAT2(assertArrayEq, EXPECTED, ACTUAL)

/// Compares a std::vector with an af::/af_array for their types, dims, and values (strict equality).
///
/// \param[in] EXPECTED_VEC The vector that represents the expected array
/// \param[in] EXPECTED_ARR_DIMS The dimensions of the expected array
/// \param[in] ACTUAL_ARR The actual resulting array from the calculation
#define ASSERT_VEC_ARRAY_EQ(EXPECTED_VEC, EXPECTED_ARR_DIMS, ACTUAL_ARR) \
    EXPECT_PRED_FORMAT3(assertArrayEq, EXPECTED_VEC, EXPECTED_ARR_DIMS, ACTUAL_ARR)

/// Compares two af::array or af_arrays for their type, dims, and values (with a given tolerance).
///
/// \param[in] EXPECTED Expected value of the assertion
/// \param[in] ACTUAL Actual value of the calculation
/// \param[in] MAX_ABSDIFF Expected maximum absolute difference between
///            elements of EXPECTED and ACTUAL
///
/// \NOTE: This macro will deallocate the af_arrays after the call
#define ASSERT_ARRAYS_NEAR(EXPECTED, ACTUAL, MAX_ABSDIFF) \
    EXPECT_PRED_FORMAT3(assertArrayNear, EXPECTED, ACTUAL, MAX_ABSDIFF)

/// Compares a std::vector with an af::array for their dims and values (with a given tolerance).
///
/// \param[in] EXPECTED_VEC The vector that represents the expected array
/// \param[in] EXPECTED_ARR_DIMS The dimensions of the expected array
/// \param[in] ACTUAL_ARR The actual array from the calculation
/// \param[in] MAX_ABSDIFF Expected maximum absolute difference between
///            elements of EXPECTED and ACTUAL
#define ASSERT_VEC_ARRAY_NEAR(EXPECTED_VEC, EXPECTED_ARR_DIMS, ACTUAL_ARR, MAX_ABSDIFF) \
    EXPECT_PRED_FORMAT4(assertArrayNear, EXPECTED_VEC, EXPECTED_ARR_DIMS, ACTUAL_ARR, \
                            MAX_ABSDIFF)


#pragma GCC diagnostic pop
