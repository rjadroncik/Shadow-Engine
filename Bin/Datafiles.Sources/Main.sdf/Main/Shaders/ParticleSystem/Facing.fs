uniform sampler2D diffuseSampler;

in vec4 properties_fs;

void main(void)
{                               
  gl_FragColor = texture2D(diffuseSampler, gl_PointCoord);
  gl_FragColor.a *= properties_fs.a;
}