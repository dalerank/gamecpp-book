const char* color = "yellow";

int main()
{
    const char *color2 = "yellow";
    printf("%p - %p, %d", color, color2, color == color2);
    
    return 0;
}
