#include "system.h"
#include "debug.h"
#include "texture.h"

// ’¸“_\‘¢‘Ì
struct VERTEX_3D
{
	Float3 Position;
	Float4 Color;
	Float2 TexCoord;
};

unsigned int tex_debug_font;

void InitDebug()
{
	tex_debug_font = LoadTexture("rom:/ball.tgafont.tga");
}

void UninitDebug()
{
	UnloadTexture(tex_debug_font);
}

void DrawPolygon(unsigned int texture,
	float x, float y, float w, float h,
	float tx, float ty, float tw, float th,
	Float4 color)
{
	VERTEX_3D vertex[4];

	float hw = w * 0.5f, hh = h * 0.5f;
	vertex[0].Position = MakeFloat3(x + hw, y - hh, 0.0f);
	vertex[1].Position = MakeFloat3(x - hw, y - hh, 0.0f);
	vertex[2].Position = MakeFloat3(x + hw, y + hh, 0.0f);
	vertex[3].Position = MakeFloat3(x - hw, y + hh, 0.0f);

	vertex[0].Color = color;
	vertex[1].Color = color;
	vertex[2].Color = color;
	vertex[3].Color = color;

	vertex[0].TexCoord = MakeFloat2(tx + tw, ty);
	vertex[1].TexCoord = MakeFloat2(tx, ty);
	vertex[2].TexCoord = MakeFloat2(tx + tw, ty + th);
	vertex[3].TexCoord = MakeFloat2(tx, ty + th);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->TexCoord);

	SetTexture(texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void DrawString(float x, float y, float size, const char* str)
{
	DrawString(x, y, size, str, MakeFloat4(1, 1, 1, 1));
}

void DrawString(float x, float y, float size, const char* str, Float4 color)
{
	float cx = x + size / 2, cy = y + size / 2;

	for (; *str; str++)
	{
		if (*str >= '0' && *str <= '9')
		{
			DrawPolygon(tex_debug_font, cx, cy, size, size, (*str - '0') * 0.1f, 0, 0.1f, 0.25f, color);
		}
		else
		if((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z'))
		{
			char c = *str <= 'Z' ? *str - 'A' : *str - 'a';
			DrawPolygon(tex_debug_font, cx, cy, size, size, c % 10 * 0.1f, (c / 10 + 1) * 0.25f, 0.1f, 0.25f, color);
		}
		else
		{
			switch (*str)
			{
			case '.':
				DrawPolygon(tex_debug_font, cx, cy, size, size, 0.6f, 0.75f, 0.1f, 0.25f, color);
				break;

			case ':':
				DrawPolygon(tex_debug_font, cx, cy, size, size, 0.7f, 0.75f, 0.1f, 0.25f, color);
				break;

			case '-':
				DrawPolygon(tex_debug_font, cx, cy, size, size, 0.8f, 0.75f, 0.1f, 0.25f, color);
				break;

			default:
				break;
			}
		}

		if (*str == '\n')
		{
			cx = x + size / 2;
			cy += size;
		}
		else
		{
			cx += size;
		}
	}
}
