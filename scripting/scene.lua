

surf_buf = RLSurfaceBuf.new()

scene = RLScene.new()


triangles = load_wavefront_obj("bunny.obj")
materials = {rrr.material(rrr.rgb(0.0, 0.0, 0.0),
                          rrr.rgb(0.0, 0.0, 0.0),
                          0.0,
                          rrr.rgb(0.0, 0.0, 0.0),
                          10,
                          rrr.rgb(0.0, 0.0, 0.0),
                          0.5)}

