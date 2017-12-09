# Capabilities
## Required

* DerivativeControl
* Image1D
* ImageBuffer
* ImageQuery
* InputAttachment
* Matrix
* Sampled1D
* SampledBuffer
* Shader

## Optional

* ClipDistance
* CullDistance
* Float64
* Geometry
* GeometryPointSize
* ImageCubeArray
* ImageGatherExtended
* ImageMSArray
* Int16
* Int64
* InterpolationFunction
* MinLod
* MultiViewport
* SampleRateShading
* SampledCubeArray
* SampledImageArrayDynamicIndexing
* SparseResidency
* StorageBufferArrayDynamicIndexing
* StorageImageArrayDynamicIndexing
* StorageImageExtendedFormats
* StorageImageMultisample
* StorageImageReadWithoutFormat
* StorageImageWriteWithoutFormat
* Tessellation
* TessellationPointSize
* UniformBufferArrayDynamicIndexing

# Ops

## Core

OpNop
OpUndef

OpSourceContinued
OpSource
OpSourceExtension
OpName
OpMemberName
OpString
OpLine
OpNoLine
OpModuleProcessed

OpDecorate
OpMemberDecorate
OpDecorationGroup
OpGroupDecorate
OpDecorateId

OpExtension
OpExtInstImport
OpExtInst

OpMemoryModel
OpEntryPoint
OpExecutionMode
OpCapability
OpExecutionModeId




## Required



## Optional

## Forbidden

### Addresses
OpSizeOf












# Accessibles:
image
buffer





# Accessor Ops










Pointers are distinct from addresses.



## OpLoad, OpStore, OpCopyMemory, OpCopyObject, OpAtomic*
Safe via SSA and lack of pointer arithmatic.

## OpAccessChain and OpInBoundsAccessChain
Run-time bounds clamping when necessary.

## OpImageTexelPointer
Run-time bounds clamping.

## OpImageFetch













# Compile time guarantees
OpAccessChain

# Clamped

### OpTypeArray
Static size



### OpTypeRuntimeArray
See OpArrayLength for getting the Length of an array of this type.
Objects of this type can only be created with OpVariable using the Uniform Storage Class.







OpImageTexelPointer

OpCopyMemory



