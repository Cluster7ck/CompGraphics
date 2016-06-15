uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;

varying vec3 Normal;
varying vec3 Position;
varying Vec2 Texcoord;

float sat(flat a) {
    return clamp(a, 0.0, 1.0);
}

void main(void) {
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos - Position);
    vec3 E = normalize(- Position);
    vex3 R = reflect(- L, N);

    vec3 DiffuseComponent = DiffColor * sat(dot(N, L));
    vec3 SpecularComponent = SpecColor * pow(sat(dot(E, R)), SpecExp);

    vec3 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord).rgb;
    DiffuseComponent *= DefuseTexColor;

    vec3 AmbientComponent = AmbientColor * DiffuseTexColor;

    gl_FragColor = vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0);
}