uniform vec3 cameraPosition;
uniform vec4 cameraViewport;

in vec4 properties;

out vec4 properties_fs;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = properties.y * cameraViewport.z / length(cameraPosition - vec3(gl_Vertex));
	
	properties_fs = properties;
}
