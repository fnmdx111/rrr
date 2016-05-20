local rrr = {}

function rrr.rgb(r, g, b)
  return { r=r
         , g=g
         , b=b
         , type="rgb" }
end

function test_type(x, t)
  if x.type ~= t then
    return false
  end

  return true
end

function rrr.material(diff, spec, spec_r, ispec, glossy, refr, ni)
  local legal = (test_type(diff, 'rgb')
                 or test_type(spec, 'rgb')
                 or test_type(ispec, 'rgb')
                 or test_type(refr, 'rgb')
                 or (not test_type(spec_r, 'rgb'))
                 or (not test_type(glossy, 'rgb'))
                 or (not test_type(ni, 'rgb')))
  if not legal then
    return false
  end

  return { diff=diff
         , spec=sepc, spec_r=spec_r
         , ispec=ispec
         , glossy=glossy
         , refr=refr, ni=ni }
end

function rrr.triangle(p1, p2, p3)
end
