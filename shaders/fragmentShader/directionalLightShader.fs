#version 330 core
//-------------------------------------------//
//此為輸入光源為Directional Light的像素著色器//
//-------------------------------------------//
/*
Directional Light為只在乎它的照射方向的光源，
不會考慮光源的位置，適合用來作為照射世界的全
域光源，例如:太陽、月亮。
*/

//輸出
out vec4 fragColor;//像素輸出顏色

//輸入
in vec3 fragPos;//像素在世界座標系統的位置
in vec3 normal;//此像素的法向量
in vec2 texCoord;//圖片在圖片座標的位置

struct Material//此像素的材質屬性
{
    //sampler2D ambient;此像素環境光的圖片，通常等於此像素漫射光的圖片
    sampler2D diffuse;//此像素漫射光的圖片
    sampler2D specular;//此像素反射光的圖片
    float shininess;//此像素反射光的亮度
}; 

uniform Material material;//此像素的物體材質特性

struct Light//光源的屬性
{
	vec3 direction;//光源對於世界座標系統的照射方向的向量

    vec3 ambient;//光源的環境光顏色    這三個的差別是光線的強度 ex.: vec3(0.1, 0.1, 0.1)
    vec3 diffuse;//光源的漫射光顏色    這三個的差別是光線的強度 ex.: vec3(0.5, 0.5, 0.5)
    vec3 specular;//光源的反射光顏色   這三個的差別是光線的強度 ex.: vec3(1.0, 1.0, 1.0)
};

uniform Light light;//照射到此像素的的光源

uniform vec3 viewPos;//鏡頭在世界座標系統的位置

void main()
{
    //此像素的漫射光的顏色
    vec3 texDiffuseColor = vec3(texture(material.diffuse, texCoord));
    //此像素的反射光的顏色
    vec3 texSpecularColor = vec3(texture(material.specular, texCoord));
    //環境光的顏色
    vec3 ambient = light.ambient * texDiffuseColor;

    //此像素的單位法向量
    vec3 norm = normalize(normal);
    //此像素到光源的單位方向向量
    vec3 lightDir = normalize(-light.direction);
    //漫射光的強度係數
    float diff = max(dot(norm, lightDir), 0.0);
    //漫射光的顏色
    vec3 diffuse = light.diffuse * diff * texDiffuseColor;
    
    //此像素到鏡頭的單位方向向量
    vec3 viewDir = normalize(viewPos - fragPos);
    //反射光的方向
    vec3 reflectDir = reflect(-lightDir, norm);
    //反射光的強度係數
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //反射光的顏色
    vec3 specular = light.specular * spec * texSpecularColor;
    //此像素的輸出顏色
    fragColor = vec4(ambient + diffuse + specular, 1.0);
    return;
}



