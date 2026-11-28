//
//  Scene.cpp
//  LightningCore
//
//  Created by Kian Marvi on 12/26/25.
//

#include "Scene.h"
#include "Component.h"
#include "Entity/Entity.h"
#include "Renderer/Metal/MetalRenderer.h"
#include <hlsl++.h>

static b3BodyType LightningRigidBody3DTypeToBox3D(RigidBody3DComponent::BodyType bodyType)
{
    switch (bodyType)
    {
        case RigidBody3DComponent::BodyType::Static:
            return b3_staticBody;
        case RigidBody3DComponent::BodyType::Dynamic:
            return b3_dynamicBody;
        case RigidBody3DComponent::BodyType::Kinematic:
            return b3_kinematicBody;
        default:
            return b3_staticBody;
    }
}

Scene::Scene()
{
}

Scene::~Scene()
{
    //b3DestroyWorld(m_WorldId);
}

Entity Scene::CreateEntity(const char* p_Tag)
{
    Entity entity = { m_Registry.create(), this};

    auto& entityTag = entity.AddComponent<TagComponent>();
    entityTag.m_Tag = (p_Tag[0] == '\0') ? "Entity" : p_Tag;
    
    return entity;
}

void Scene::OnRuntimeStart()
{
    m_PhysicsWorld = b3DefaultWorldDef();
    m_PhysicsWorld.gravity = (b3Vec3){ 0.0f, -9.8f, 0.0f };
    
    m_WorldId = b3CreateWorld(&m_PhysicsWorld);
    
    auto view = m_Registry.view<RigidBody3DComponent>();
    for (const auto& e: view)
    {
        Entity entity {e , this};
        auto &transform = entity.GetComponent<TransformComponent>();
        auto &rb3d      = entity.GetComponent<RigidBody3DComponent>();
        
        b3BodyDef bodyDef;
        bodyDef.type = LightningRigidBody3DTypeToBox3D(rb3d.GetRigidBodyType());
    }
}

void Scene::OnUpdateRuntime(float dt)
{
    
}

void Scene::OnRuntimeStop()
{
}

template <typename Renderer>
void Scene::RenderScene(Renderer &renderer, const float aspectRatio)
{
    
    auto textured_meshes = m_Registry.view<TransformComponent, MeshComponent, TextureComponent>();
    auto light_sources = m_Registry.view<TransformComponent, MeshComponent, LightComponent>();
    auto meshes = m_Registry.view<TransformComponent, MeshComponent>(entt::exclude<TextureComponent, LightComponent>);
    auto skybox = m_Registry.view<TransformComponent, MeshComponent, TextureComponent, SkyboxComponent>();
    auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
    
    Camera* mainCamera = nullptr;
    
    for (const auto& entity : cameras)
    {
        const auto &[transform, camera] = cameras.get<TransformComponent, CameraComponent>(entity);
        
        if (camera.b_Primary)
        {
            mainCamera = &camera.m_Camera;
            break;
        }
    }
    
    if (!mainCamera)
        return;
   
    renderer.Submit(*mainCamera, aspectRatio);
    
    LightComponent lightsActive;
    
    if (m_Registry.storage<LightComponent>().size() != 0)
    {
        auto light_entity = light_sources.front();
        lightsActive = light_sources.get<LightComponent>(light_entity);
        for (const auto &entity : light_sources)
        {
            const auto &[transform, lights, mesh] = light_sources.get<TransformComponent, LightComponent, MeshComponent>(entity);
            renderer.RenderLights(mul(transform.GetScale(), transform.GetTranslation()), mesh.m_MeshHandle, lights);
        }
    }
    
    std::vector<float4x4> meshTransforms;
    
    for (const auto &entity : textured_meshes)
    {
        const auto &[transform, textures, mesh] = textured_meshes.get<TransformComponent, TextureComponent, MeshComponent>(entity);
        
        if (mesh.m_Transforms.empty())
        {
            meshTransforms.clear();
            float4x4 scaleMatrix = transform.GetScale();
            float4x4 rotationMatrix = float4x4::rotation_axis(transform.GetRotation(), transform.GetRotationAngle());
            float4x4 translationMatrix = transform.GetTranslation();
            
            float4x4 sr = mul(scaleMatrix, rotationMatrix);
            float4x4 modelMatrix = mul(sr, translationMatrix);
            meshTransforms.emplace_back(modelMatrix);
            renderer.RenderMesh(meshTransforms, mesh.m_MeshHandle, textures.texture, lightsActive);
        }
        else
        {
            renderer.RenderMesh(mesh.m_Transforms, mesh.m_MeshHandle, textures.texture, lightsActive);
        }
    }
    
    for (const auto &entity : meshes)
    {
        const auto &[transform, mesh] = meshes.get<TransformComponent, MeshComponent>(entity);
        
        if (mesh.m_Transforms.empty())
        {
            meshTransforms.clear();
            float4x4 scaleMatrix = transform.GetScale();
            float4x4 rotationMatrix = float4x4::rotation_axis(transform.GetRotation(), transform.GetRotationAngle());
            float4x4 translationMatrix = transform.GetTranslation();
            
            float4x4 sr = mul(scaleMatrix, rotationMatrix);
            float4x4 modelMatrix = mul(sr, translationMatrix);
            meshTransforms.emplace_back(modelMatrix);
            renderer.RenderMesh(meshTransforms, mesh.m_MeshHandle, lightsActive);
        }
        else
        {
            renderer.RenderMesh(mesh.m_Transforms, mesh.m_MeshHandle, lightsActive);
        }
    }
    
    for (const auto &entity: skybox)
    {
        const auto &[transform, textures, mesh] = skybox.get<TransformComponent, TextureComponent, MeshComponent>(entity);
        float4x4 scaleMatrix = transform.GetScale();
        float4x4 rotationMatrix = float4x4::rotation_axis(transform.GetRotation(), transform.GetRotationAngle());
        float4x4 translationMatrix = transform.GetTranslation();
        
        float4x4 sr = mul(scaleMatrix, rotationMatrix);
        float4x4 modelMatrix = mul(sr, translationMatrix);
        renderer.RenderSkybox(modelMatrix, mesh.m_MeshHandle, textures.texture);
    }
    
    renderer.Commit();
}

template void Scene::RenderScene<MetalRenderer>(MetalRenderer&, const float);
