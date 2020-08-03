#pragma once

#include "MikoEngine/BaseHeader.h"
#include "MikoEngine/CoreHeader.h"
#include "MikoEngine/UtilityHeader.h"
#include "MikoEngine/MathHeader.h"
#include "MikoEngine/RenderHeader.h"
#include "MikoEngine/GraphicsHeader.h"
#include "MikoEngine/SceneHeader.h"
#include "MikoEngine/ApplicationHeader.h"

// Uniform buffer data structure.
struct Transforms
{
    SE_ALIGNED(16)
        glm::mat4 model;
    SE_ALIGNED(16)
        glm::mat4 view;
    SE_ALIGNED(16)
        glm::mat4 projection;
};

class MyApp final : public Application
{
public:
    bool init(int argc, const char* argv[]) override
    {
        // Create GPU resources.
        if ( !create_shaders() )
            return false;

        if ( !create_uniform_buffer() )
            return false;

        // Load mesh.
        if ( !load_mesh() )
            return false;

        create_descriptor_set_layout();
        create_descriptor_set();
        create_pipeline_state();

        // Create camera.
        create_camera();

        return true;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void update(double delta) override
    {
        vk::CommandBuffer::Ptr cmd_buf = m_vk_backend->allocate_graphics_command_buffer();

        VkCommandBufferBeginInfo begin_info;
        SE_ZERO_MEMORY(begin_info);

        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(cmd_buf->handle(), &begin_info);

        {
            SE_SCOPED_SAMPLE("update", cmd_buf);

            // Render profiler.
            profiler::ui();

            // Update camera.
            m_main_camera->update();

            // Update uniforms.
            update_uniforms(cmd_buf);

            // Render.
            render(cmd_buf);
        }

        vkEndCommandBuffer(cmd_buf->handle());

        submit_and_present({ cmd_buf });
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void shutdown() override
    {
        m_mesh.reset();
        m_pso.reset();
        m_pipeline_layout.reset();
        m_per_frame_ds_layout.reset();
        m_per_frame_ds.reset();
        m_ubo.reset();
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    ApplicationSetting intial_app_settings() override
    {
        // Set custom settings here...
        ApplicationSetting settings;

        settings.width = 1280;
        settings.height = 720;
        settings.title = "Hello Game (Vulkan)";
        settings.ray_tracing = false;

        return settings;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void window_resized(int width, int height) override
    {
        // Override window resized method to update camera projection.
        m_main_camera->update_projection(60.0f, 0.1f, 1000.0f, float(m_width) / float(m_height));
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

private:
    // -----------------------------------------------------------------------------------------------------------------------------------

    bool create_shaders()
    {
        return true;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    bool create_uniform_buffer()
    {
        m_ubo_size = m_vk_backend->aligned_dynamic_ubo_size(sizeof(Transforms));
        m_ubo = vk::Buffer::create(m_vk_backend, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, m_ubo_size * vk::Backend::kMaxFramesInFlight, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);

        return true;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void create_descriptor_set_layout()
    {
        vk::DescriptorSetLayout::Desc desc;

        desc.add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_VERTEX_BIT);

        m_per_frame_ds_layout = vk::DescriptorSetLayout::create(m_vk_backend, desc);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void create_descriptor_set()
    {
        m_per_frame_ds = m_vk_backend->allocate_descriptor_set(m_per_frame_ds_layout);

        VkDescriptorBufferInfo buffer_info;

        buffer_info.buffer = m_ubo->handle();
        buffer_info.offset = 0;
        buffer_info.range = sizeof(glm::mat4);

        VkWriteDescriptorSet write_data;
        SE_ZERO_MEMORY(write_data);

        write_data.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_data.descriptorCount = 1;
        write_data.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        write_data.pBufferInfo = &buffer_info;
        write_data.dstBinding = 0;
        write_data.dstSet = m_per_frame_ds->handle();

        vkUpdateDescriptorSets(m_vk_backend->device(), 1, &write_data, 0, nullptr);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void create_pipeline_state()
    {
        // ---------------------------------------------------------------------------
        // Create shader modules
        // ---------------------------------------------------------------------------

        vk::ShaderModule::Ptr vs = vk::ShaderModule::create_from_file(m_vk_backend, "shaders/mesh.vert.spv");
        vk::ShaderModule::Ptr fs = vk::ShaderModule::create_from_file(m_vk_backend, "shaders/mesh.frag.spv");

        vk::GraphicsPipeline::Desc pso_desc;

        pso_desc.add_shader_stage(VK_SHADER_STAGE_VERTEX_BIT, vs, "main")
            .add_shader_stage(VK_SHADER_STAGE_FRAGMENT_BIT, fs, "main");

        // ---------------------------------------------------------------------------
        // Create vertex input state
        // ---------------------------------------------------------------------------

        pso_desc.set_vertex_input_state(m_mesh->vertex_input_state_desc());

        // ---------------------------------------------------------------------------
        // Create pipeline input assembly state
        // ---------------------------------------------------------------------------

        vk::InputAssemblyStateDesc input_assembly_state_desc;

        input_assembly_state_desc.set_primitive_restart_enable(false)
            .set_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        pso_desc.set_input_assembly_state(input_assembly_state_desc);

        // ---------------------------------------------------------------------------
        // Create viewport state
        // ---------------------------------------------------------------------------

        vk::ViewportStateDesc vp_desc;

        vp_desc.add_viewport(0.0f, 0.0f, m_width, m_height, 0.0f, 1.0f)
            .add_scissor(0, 0, m_width, m_height);

        pso_desc.set_viewport_state(vp_desc);

        // ---------------------------------------------------------------------------
        // Create rasterization state
        // ---------------------------------------------------------------------------

        vk::RasterizationStateDesc rs_state;

        rs_state.set_depth_clamp(VK_FALSE)
            .set_rasterizer_discard_enable(VK_FALSE)
            .set_polygon_mode(VK_POLYGON_MODE_FILL)
            .set_line_width(1.0f)
            .set_cull_mode(VK_CULL_MODE_BACK_BIT)
            .set_front_face(VK_FRONT_FACE_COUNTER_CLOCKWISE)
            .set_depth_bias(VK_FALSE);

        pso_desc.set_rasterization_state(rs_state);

        // ---------------------------------------------------------------------------
        // Create multisample state
        // ---------------------------------------------------------------------------

        vk::MultisampleStateDesc ms_state;

        ms_state.set_sample_shading_enable(VK_FALSE)
            .set_rasterization_samples(VK_SAMPLE_COUNT_1_BIT);

        pso_desc.set_multisample_state(ms_state);

        // ---------------------------------------------------------------------------
        // Create depth stencil state
        // ---------------------------------------------------------------------------

        vk::DepthStencilStateDesc ds_state;

        ds_state.set_depth_test_enable(VK_TRUE)
            .set_depth_write_enable(VK_TRUE)
            .set_depth_compare_op(VK_COMPARE_OP_LESS)
            .set_depth_bounds_test_enable(VK_FALSE)
            .set_stencil_test_enable(VK_FALSE);

        pso_desc.set_depth_stencil_state(ds_state);

        // ---------------------------------------------------------------------------
        // Create color blend state
        // ---------------------------------------------------------------------------

        vk::ColorBlendAttachmentStateDesc blend_att_desc;

        blend_att_desc.set_color_write_mask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
            .set_blend_enable(VK_FALSE);

        vk::ColorBlendStateDesc blend_state;

        blend_state.set_logic_op_enable(VK_FALSE)
            .set_logic_op(VK_LOGIC_OP_COPY)
            .set_blend_constants(0.0f, 0.0f, 0.0f, 0.0f)
            .add_attachment(blend_att_desc);

        pso_desc.set_color_blend_state(blend_state);

        // ---------------------------------------------------------------------------
        // Create pipeline layout
        // ---------------------------------------------------------------------------

        vk::PipelineLayout::Desc pl_desc;

        pl_desc.add_descriptor_set_layout(m_per_frame_ds_layout)
            .add_descriptor_set_layout(Material::pbr_descriptor_set_layout());

        m_pipeline_layout = vk::PipelineLayout::create(m_vk_backend, pl_desc);

        pso_desc.set_pipeline_layout(m_pipeline_layout);

        // ---------------------------------------------------------------------------
        // Create dynamic state
        // ---------------------------------------------------------------------------

        pso_desc.add_dynamic_state(VK_DYNAMIC_STATE_VIEWPORT)
            .add_dynamic_state(VK_DYNAMIC_STATE_SCISSOR);

        // ---------------------------------------------------------------------------
        // Create pipeline
        // ---------------------------------------------------------------------------

        pso_desc.set_render_pass(m_vk_backend->swapchain_render_pass());

        m_pso = vk::GraphicsPipeline::create(m_vk_backend, pso_desc);
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    bool load_mesh()
    {
        m_mesh = Mesh::load(m_vk_backend, "teapot.obj");
        return m_mesh != nullptr;
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void create_camera()
    {
        m_main_camera = std::make_unique<Camera>(
            60.0f, 0.1f, 1000.0f, float(m_width) / float(m_height), glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0, -1.0f));
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void render(vk::CommandBuffer::Ptr cmd_buf)
    {
        SE_SCOPED_SAMPLE("render", cmd_buf);

        VkClearValue clear_values[2];

        clear_values[0].color.float32[0] = 0.0f;
        clear_values[0].color.float32[1] = 0.0f;
        clear_values[0].color.float32[2] = 0.0f;
        clear_values[0].color.float32[3] = 1.0f;

        clear_values[1].color.float32[0] = 1.0f;
        clear_values[1].color.float32[1] = 1.0f;
        clear_values[1].color.float32[2] = 1.0f;
        clear_values[1].color.float32[3] = 1.0f;

        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = m_vk_backend->swapchain_render_pass()->handle();
        info.framebuffer = m_vk_backend->swapchain_framebuffer()->handle();
        info.renderArea.extent.width = m_width;
        info.renderArea.extent.height = m_height;
        info.clearValueCount = 2;
        info.pClearValues = &clear_values[0];

        vkCmdBeginRenderPass(cmd_buf->handle(), &info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmd_buf->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pso->handle());

        VkViewport vp;

        vp.x = 0.0f;
        vp.y = (float)m_height;
        vp.width = (float)m_width;
        vp.height = -(float)m_height;
        vp.minDepth = 0.0f;
        vp.maxDepth = 1.0f;

        vkCmdSetViewport(cmd_buf->handle(), 0, 1, &vp);

        VkRect2D scissor_rect;

        scissor_rect.extent.width = m_width;
        scissor_rect.extent.height = m_height;
        scissor_rect.offset.x = 0;
        scissor_rect.offset.y = 0;

        vkCmdSetScissor(cmd_buf->handle(), 0, 1, &scissor_rect);

        const uint32_t dynamic_offset = m_ubo_size * m_vk_backend->current_frame_idx();

        vkCmdBindDescriptorSets(cmd_buf->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout->handle(), 0, 1, &m_per_frame_ds->handle(), 1, &dynamic_offset);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(cmd_buf->handle(), 0, 1, &m_mesh->vertex_buffer()->handle(), &offset);
        vkCmdBindIndexBuffer(cmd_buf->handle(), m_mesh->index_buffer()->handle(), 0, VK_INDEX_TYPE_UINT32);

        for ( uint32_t i = 0; i < m_mesh->sub_mesh_count(); i++ )
        {
            auto& submesh = m_mesh->sub_meshes()[i];
            auto& mat = m_mesh->material(submesh.mat_idx);

            vkCmdBindDescriptorSets(cmd_buf->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout->handle(), 1, 1, &mat->pbr_descriptor_set()->handle(), 0, nullptr);

            // Issue draw call.
            vkCmdDrawIndexed(cmd_buf->handle(), submesh.index_count, 1, submesh.base_index, submesh.base_vertex, 0);
        }

        m_debug_draw.sphere(10.0f, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_debug_draw.render(m_vk_backend, cmd_buf, m_width, m_height, m_main_camera->m_view_projection, m_main_camera->m_position);

        render_gui(cmd_buf);

        vkCmdEndRenderPass(cmd_buf->handle());
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

    void update_uniforms(vk::CommandBuffer::Ptr cmd_buf)
    {
        SE_SCOPED_SAMPLE("update_uniforms", cmd_buf);

        m_transforms.model = glm::mat4(1.0f);
        m_transforms.model = glm::translate(m_transforms.model, glm::vec3(0.0f, -20.0f, 0.0f));
        m_transforms.model = glm::rotate(m_transforms.model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        m_transforms.model = glm::scale(m_transforms.model, glm::vec3(0.6f));
        m_transforms.view = m_main_camera->m_view;
        m_transforms.projection = m_main_camera->m_projection;

        uint8_t* ptr = (uint8_t*)m_ubo->mapped_ptr();
        memcpy(ptr + m_ubo_size * m_vk_backend->current_frame_idx(), &m_transforms, sizeof(Transforms));
    }

    // -----------------------------------------------------------------------------------------------------------------------------------

private:
    // GPU resources.
    size_t                           m_ubo_size;
    vk::GraphicsPipeline::Ptr    m_pso;
    vk::PipelineLayout::Ptr      m_pipeline_layout;
    vk::DescriptorSetLayout::Ptr m_per_frame_ds_layout;
    vk::DescriptorSet::Ptr       m_per_frame_ds;
    vk::Buffer::Ptr              m_ubo;

    // Camera.
    std::unique_ptr<Camera> m_main_camera;

    // Assets.
    Mesh::Ptr m_mesh;

    // Uniforms.
    Transforms m_transforms;
};