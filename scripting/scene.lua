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

scene = RLScene.new(surf_buf, light_buf, mtr_buf, cam)

print(scene:surf_buf())
print(scene:surf_buf())
print(scene:surf_buf():add_surface())


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

