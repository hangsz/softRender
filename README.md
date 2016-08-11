# softRender
**采用光线追踪的方法渲染一个立方体，未经过性能优化。**
## Class Design：
#### `Point` 点定义及操作接口
#### `Vector` 向量定义及操作接口
#### `Vertex` 顶点定义及操作接口
#### `Transform` 坐标变换矩阵定义以及顶点变换的接口
#### `Render`渲染类，参数控制及算法
###
    维护 VertexBuffer, textureBuffer,frameBuffer, zBuffer
    提供 vertexShader,fragmentShader 接口
