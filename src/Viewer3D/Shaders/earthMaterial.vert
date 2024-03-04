VARYING vec2 uv;

float getHeight(vec2 pos)
{
    return texture(someTextureMap, pos).g;
}

void MAIN()
{
    uv = UV0;
    VERTEX += 100 * NORMAL * getHeight(UV0);
}
