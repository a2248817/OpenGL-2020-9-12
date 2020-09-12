#version 330 core
//-------------------------------------------//
//������J������Directional Light�������ۦ⾹//
//-------------------------------------------//
/*
Directional Light���u�b�G�����Ӯg��V�������A
���|�Ҽ{��������m�A�A�X�Ψӧ@���Ӯg�@�ɪ���
������A�Ҧp:�Ӷ��B��G�C
*/

//��X
out vec4 fragColor;//������X�C��

//��J
in vec3 fragPos;//�����b�@�ɮy�Шt�Ϊ���m
in vec3 normal;//���������k�V�q
in vec2 texCoord;//�Ϥ��b�Ϥ��y�Ъ���m

struct Material//�������������ݩ�
{
    //sampler2D ambient;���������ҥ����Ϥ��A�q�`���󦹹������g�����Ϥ�
    sampler2D diffuse;//���������g�����Ϥ�
    sampler2D specular;//�������Ϯg�����Ϥ�
    float shininess;//�������Ϯg�����G��
}; 

uniform Material material;//���������������S��

struct Light//�������ݩ�
{
	vec3 direction;//�������@�ɮy�Шt�Ϊ��Ӯg��V���V�q

    vec3 ambient;//���������ҥ��C��    �o�T�Ӫ��t�O�O���u���j�� ex.: vec3(0.1, 0.1, 0.1)
    vec3 diffuse;//���������g���C��    �o�T�Ӫ��t�O�O���u���j�� ex.: vec3(0.5, 0.5, 0.5)
    vec3 specular;//�������Ϯg���C��   �o�T�Ӫ��t�O�O���u���j�� ex.: vec3(1.0, 1.0, 1.0)
};

uniform Light light;//�Ӯg�즹������������

uniform vec3 viewPos;//���Y�b�@�ɮy�Шt�Ϊ���m

void main()
{
    //�����������g�����C��
    vec3 texDiffuseColor = vec3(texture(material.diffuse, texCoord));
    //���������Ϯg�����C��
    vec3 texSpecularColor = vec3(texture(material.specular, texCoord));
    //���ҥ����C��
    vec3 ambient = light.ambient * texDiffuseColor;

    //�����������k�V�q
    vec3 norm = normalize(normal);
    //�����������������V�V�q
    vec3 lightDir = normalize(-light.direction);
    //���g�����j�׫Y��
    float diff = max(dot(norm, lightDir), 0.0);
    //���g�����C��
    vec3 diffuse = light.diffuse * diff * texDiffuseColor;
    
    //�����������Y������V�V�q
    vec3 viewDir = normalize(viewPos - fragPos);
    //�Ϯg������V
    vec3 reflectDir = reflect(-lightDir, norm);
    //�Ϯg�����j�׫Y��
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //�Ϯg�����C��
    vec3 specular = light.specular * spec * texSpecularColor;
    //����������X�C��
    fragColor = vec4(ambient + diffuse + specular, 1.0);
    return;
}



