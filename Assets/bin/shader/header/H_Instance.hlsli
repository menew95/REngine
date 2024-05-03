#ifndef H_INSTNACE

#define H_INSTANCE

    #define INSTANCED_ARRAY_SIZE  500

    cbuffer Instancing_PerObjectbuf
    {
        struct {
            float4x4 _world;
            float4x4 _worldInv;
    #if USE_RENDERINGLAYER_ARRAY
            float _layerArray;
    #endif
        } arrPerDraw0Array[INSTANCED_ARRAY_SIZE];
    }
#endif