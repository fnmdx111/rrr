function v3(x, y, z)
    return { x = x
           , y = y
           , z = z }
end

cam = RLCamera.new(v3(3, 3, 3),
                   v3(-0.7, -0.7, -0.7),
                   35.0, 35.0, 25.0, 400, 300)

surf_buf = RLSurfaceBuf.new()
light_buf = RLLightBuf.new()
mtr_buf = RLMaterialBuf.new()

surf_buf:add_triangle(
    {x=1, y=1, z=1},
    {x=2, y=2, z=2},
    {x=3, y=3, z=3}
)
surf_buf:add_sphere(v3(1, 1, 1), 5)

light_buf:add_point_light(v3(1, 1, 1), v3(0.2, 0.2, 0.2))

scene = RLScene.new()
scene:install_surf_buf(surf_buf)
scene:install_light_buf(light_buf)
scene:install_material_buf(mtr_buf)
scene:install_camera(cam)

surf_buf:append_wavefront_object('bunny_regular.obj')

mtr_buf:add_phongbling(
    v3(0.7, 0.0, 0.0),
    v3(0.5, 0.5, 0.5), 100,
    v3(0.0, 0.0, 0.0),
    50,
    v3(1.0, 1.0, 1.0), 1.25)


scene:render()
--
--scene:install_surf_buf(surf_buf)

--
--triangles = load_wavefront_obj("bunny.obj")
--materials = {rrr.material(rrr.rgb(0.0, 0.0, 0.0),
--                          rrr.rgb(0.0, 0.0, 0.0),
--                          0.0,
--                          rrr.rgb(0.0, 0.0, 0.0),
--                          10,
--                          rrr.rgb(0.0, 0.0, 0.0),
--                          0.5)}

