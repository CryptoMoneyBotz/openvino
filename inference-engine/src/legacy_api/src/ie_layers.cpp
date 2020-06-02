// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <ie_layers.h>

using namespace InferenceEngine;

const DataPtr CNNLayer::input() const {
    if (insData.empty()) {
        THROW_IE_EXCEPTION << "Internal error: input data is empty";
    }
    auto lockedFirstInsData = insData[0].lock();
    if (!lockedFirstInsData) {
        THROW_IE_EXCEPTION << "Internal error: unable to lock weak_ptr\n";
    }
    return lockedFirstInsData;
}

float CNNLayer::ie_parse_float(const std::string& str) {
    if (str == "-inf") {
        return -std::numeric_limits<float>::infinity();
    } else if (str == "inf") {
        return std::numeric_limits<float>::infinity();
    } else {
        float res;
        std::stringstream val_stream(str);
        val_stream.imbue(std::locale("C"));
        val_stream >> res;
        if (!val_stream.eof()) THROW_IE_EXCEPTION;
        return res;
    }
}

std::string CNNLayer::ie_serialize_float(float value) {
    std::stringstream val_stream;
    val_stream.imbue(std::locale("C"));
    val_stream << value;
    return val_stream.str();
}

float CNNLayer::GetParamAsFloat(const char* param, float def) const {
    std::string val = GetParamAsString(param, ie_serialize_float(def).c_str());
    try {
        return ie_parse_float(val);
    } catch (...) {
        THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " from IR for layer " << name << ". Value "
                           << val << " cannot be casted to float.";
    }
}

float CNNLayer::GetParamAsFloat(const char* param) const {
    std::string val = GetParamAsString(param);
    try {
        return ie_parse_float(val);
    } catch (...) {
        THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " from IR for layer " << name << ". Value "
                           << val << " cannot be casted to float.";
    }
}

std::vector<float> CNNLayer::GetParamAsFloats(const char* param, std::vector<float> def) const {
    std::string vals = GetParamAsString(param, "");
    std::vector<float> result;
    std::istringstream stream(vals);
    std::string str;
    if (vals.empty()) return def;
    while (getline(stream, str, ',')) {
        try {
            float val = ie_parse_float(str);
            result.push_back(val);
        } catch (...) {
            THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " " << str << " from IR for layer " << name
                               << ". Value " << vals << " cannot be casted to floats.";
        }
    }
    return result;
}

std::vector<float> CNNLayer::GetParamAsFloats(const char* param) const {
    std::string vals = GetParamAsString(param);
    std::vector<float> result;
    std::istringstream stream(vals);
    std::string str;
    while (getline(stream, str, ',')) {
        try {
            float val = ie_parse_float(str);
            result.push_back(val);
        } catch (...) {
            THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " " << str << " from IR for layer " << name
                               << ". Value " << vals << " cannot be casted to floats.";
        }
    }
    return result;
}

int CNNLayer::GetParamAsInt(const char* param, int def) const {
    std::string val = GetParamAsString(param, std::to_string(def).c_str());
    try {
        return std::stoi(val);
    } catch (...) {
        THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " from IR for layer " << name << ". Value "
                           << val << " cannot be casted to int.";
    }
}

int CNNLayer::GetParamAsInt(const char* param) const {
    std::string val = GetParamAsString(param);
    try {
        return std::stoi(val);
    } catch (...) {
        THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " from IR for layer " << name << ". Value "
                           << val << " cannot be casted to int.";
    }
}

std::vector<int> CNNLayer::GetParamAsInts(const char* param, std::vector<int> def) const {
    std::string vals = GetParamAsString(param, "");
    std::vector<int> result;
    std::istringstream stream(vals);
    std::string str;
    if (vals.empty()) return def;
    while (getline(stream, str, ',')) {
        try {
            result.push_back(std::stoi(str));
        } catch (...) {
            THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " " << str << " from IR for layer " << name
                               << ". Value " << vals << " cannot be casted to int.";
        }
    }
    return result;
}

std::vector<int> CNNLayer::GetParamAsInts(const char* param) const {
    std::string vals = GetParamAsString(param);
    std::vector<int> result;
    std::istringstream stream(vals);
    std::string str;
    while (getline(stream, str, ',')) {
        try {
            result.push_back(std::stoi(str));
        } catch (...) {
            THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " " << str << " from IR for layer " << name
                               << ". Value " << vals << " cannot be casted to int.";
        }
    }
    return result;
}

unsigned int CNNLayer::GetParamAsUInt(const char* param, unsigned int def) const {
    std::string val = GetParamAsString(param, std::to_string(def).c_str());
    std::string message = "Cannot parse parameter " + std::string(param) + " from IR for layer " + name +
                          ". Value " + val + " cannot be casted to int.";
    try {
        int value = std::stoi(val);
        if (value < 0) {
            THROW_IE_EXCEPTION << message;
        }
        return static_cast<unsigned int>(value);
    } catch (...) {
        THROW_IE_EXCEPTION << message;
    }
}

unsigned int CNNLayer::GetParamAsUInt(const char* param) const {
    std::string val = GetParamAsString(param);
    std::string message = "Cannot parse parameter " + std::string(param) + " from IR for layer " + name +
                          ". Value " + val + " cannot be casted to unsigned int.";
    try {
        int value = std::stoi(val);
        if (value < 0) {
            THROW_IE_EXCEPTION << message;
        }
        return static_cast<unsigned int>(value);
    } catch (...) {
        THROW_IE_EXCEPTION << message;
    }
}

std::vector<unsigned int> CNNLayer::GetParamAsUInts(const char* param, std::vector<unsigned int> def) const {
    std::string vals = GetParamAsString(param, "");
    std::vector<unsigned int> result;
    std::istringstream stream(vals);
    std::string str;
    std::string message = "Cannot parse parameter " + std::string(param) + " " + str + " from IR for layer " +
                          name + ". Value " + vals + " cannot be casted to unsigned int.";
    if (vals.empty()) return def;
    while (getline(stream, str, ',')) {
        try {
            int value = std::stoi(str);
            if (value < 0) {
                THROW_IE_EXCEPTION << message;
            }
            result.push_back(static_cast<unsigned int>(value));
        } catch (...) {
            THROW_IE_EXCEPTION << message;
        }
    }
    return result;
}

std::vector<unsigned int> CNNLayer::GetParamAsUInts(const char* param) const {
    std::string vals = GetParamAsString(param);
    std::vector<unsigned int> result;
    std::istringstream stream(vals);
    std::string str;
    std::string message = "Cannot parse parameter " + std::string(param) + " " + str + " from IR for layer " +
                          name + ". Value " + vals + " cannot be casted to int.";
    while (getline(stream, str, ',')) {
        try {
            int value = std::stoi(str);
            if (value < 0) {
                THROW_IE_EXCEPTION << message;
            }
            result.push_back(static_cast<unsigned int>(value));
        } catch (...) {
            THROW_IE_EXCEPTION << message;
        }
    }
    return result;
}

bool CNNLayer::GetParamAsBool(const char* param, bool def) const {
    std::string val = GetParamAsString(param, std::to_string(def).c_str());
    std::string loweredCaseValue;
    std::transform(val.begin(), val.end(), std::back_inserter(loweredCaseValue), [](char value) {
        return static_cast<char>(std::tolower(value));
    });

    bool result = false;

    if (!(std::istringstream(loweredCaseValue) >> std::boolalpha >> result)) {
        // attempting parse using non alpha bool
        return (GetParamAsInt(param, def) != 0);
    }

    return result;
}

bool CNNLayer::GetParamAsBool(const char* param) const {
    std::string val = GetParamAsString(param);
    std::string loweredCaseValue;
    std::transform(val.begin(), val.end(), std::back_inserter(loweredCaseValue), [](char value) {
        return static_cast<char>(std::tolower(value));
    });

    bool result = false;

    if (!(std::istringstream(loweredCaseValue) >> std::boolalpha >> result)) {
        // attempting parse using non alpha bool
        return (GetParamAsInt(param) != 0);
    }

    return result;
}

std::string CNNLayer::GetParamAsString(const char* param, const char* def) const {
    auto it = params.find(param);
    if (it == params.end() || it->second.empty()) {
        return def;
    }
    return (*it).second;
}

bool CNNLayer::CheckParamPresence(const char* param) const {
    auto it = params.find(param);
    if (it == params.end()) {
        return false;
    }
    return true;
}

std::string CNNLayer::GetParamAsString(const char* param) const {
    auto it = params.find(param);
    if (it == params.end()) {
        THROW_IE_EXCEPTION << "No such parameter name '" << param << "' for layer " << name;
    }
    return (*it).second;
}

std::vector<std::string> CNNLayer::GetParamAsStrings(const char* param, std::vector<std::string> def) const {
    std::string vals = GetParamAsString(param, "");
    std::vector<std::string> result;
    std::istringstream stream(vals);
    std::string str;
    if (vals.empty()) return def;
    while (getline(stream, str, ',')) {
        try {
            result.push_back(str);
        } catch (...) {
            THROW_IE_EXCEPTION << "Cannot parse parameter " << param << " from IR for layer " << name << ".";
        }
    }
    return result;
}

CNNLayer::~CNNLayer() {}
WeightableLayer::~WeightableLayer() {}
ConvolutionLayer::~ConvolutionLayer() {}
DeconvolutionLayer::~DeconvolutionLayer() {}
DeformableConvolutionLayer::~DeformableConvolutionLayer() {}
PoolingLayer::~PoolingLayer() {}
BinaryConvolutionLayer::~BinaryConvolutionLayer() {}
FullyConnectedLayer::~FullyConnectedLayer() {}
ConcatLayer::~ConcatLayer() {}
SplitLayer::~SplitLayer() {}
NormLayer::~NormLayer() {}
SoftMaxLayer::~SoftMaxLayer() {}
GRNLayer::~GRNLayer() {}
MVNLayer::~MVNLayer() {}
ReLULayer::~ReLULayer() {}
ClampLayer::~ClampLayer() {}
ReLU6Layer::~ReLU6Layer() {}
EltwiseLayer::~EltwiseLayer() {}
CropLayer::~CropLayer() {}
ReshapeLayer::~ReshapeLayer() {}
TileLayer::~TileLayer() {}
ScaleShiftLayer::~ScaleShiftLayer() {}
TensorIterator::~TensorIterator() {}
RNNCellBase::~RNNCellBase() {}
LSTMCell::~LSTMCell() {}
GRUCell::~GRUCell() {}
RNNCell::~RNNCell() {}
RNNSequenceLayer::~RNNSequenceLayer() {}
PReLULayer::~PReLULayer() {}
PowerLayer::~PowerLayer() {}
BatchNormalizationLayer::~BatchNormalizationLayer() {}
GemmLayer::~GemmLayer() {}
PadLayer::~PadLayer() {}
GatherLayer::~GatherLayer() {}
StridedSliceLayer::~StridedSliceLayer() {}
ShuffleChannelsLayer::~ShuffleChannelsLayer() {}
DepthToSpaceLayer::~DepthToSpaceLayer() {}
SpaceToDepthLayer::~SpaceToDepthLayer() {}
SpaceToBatchLayer::~SpaceToBatchLayer() {}
BatchToSpaceLayer::~BatchToSpaceLayer() {}
SparseFillEmptyRowsLayer::~SparseFillEmptyRowsLayer() {}
SparseSegmentReduceLayer::~SparseSegmentReduceLayer() {}
ExperimentalSparseWeightedReduceLayer::~ExperimentalSparseWeightedReduceLayer() {}
SparseToDenseLayer::~SparseToDenseLayer() {}
BucketizeLayer::~BucketizeLayer() {}
ReverseSequenceLayer::~ReverseSequenceLayer() {}
OneHotLayer::~OneHotLayer() {}
RangeLayer::~RangeLayer() {}
FillLayer::~FillLayer() {}
SelectLayer::~SelectLayer() {}
BroadcastLayer::~BroadcastLayer() {}
QuantizeLayer::~QuantizeLayer() {}
MathLayer::~MathLayer() {}
ReduceLayer::~ReduceLayer() {}
TopKLayer::~TopKLayer() {}
UniqueLayer::~UniqueLayer() {}
NonMaxSuppressionLayer::~NonMaxSuppressionLayer() {}
ScatterLayer::~ScatterLayer() {}
ExperimentalDetectronPriorGridGeneratorLayer::~ExperimentalDetectronPriorGridGeneratorLayer() {}
ExperimentalDetectronGenerateProposalsSingleImageLayer::~ExperimentalDetectronGenerateProposalsSingleImageLayer() {}
