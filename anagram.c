#include<stdio.h>
#include<stdlib.h>

#define MAX_WORD_LEN (64)

void usage(const char *name)
{
  fprintf(stderr, "%s: Anagrammer\n", name);
}

void error_usage(const char *name, int err)
{
  usage(name);
  exit(err);
}

int main(int argc, char *argv[])
{
  char *srch, match[16], word[16];
  int count[26], ci, wild = 0, finding = 1, pos = 0;

  if ( argc < 2 )
    error_usage(argv[0], 1);

  for(ci = 0; ci < 26; ci++) count[ci] = 0;

  srch = argv[1];

  while(*srch != '\0')
  {
    ci = 1;
    if ( *srch >= '0' && *srch <= '9' )
    {
      ci = 0;
      while(*srch >= '0' && *srch <= '9')
        ci = ci * 10 + *(srch++) - '0';
    }
    if ( *srch >= 'a' && *srch <= 'z' ) count[*srch - 'a'] += ci;
    else if ( *srch >= 'A' && *srch <= 'Z' ) count[*srch - 'A'] += ci;
    else if ( *srch == '.' || *srch == '_' ) wild += ci;
    else error_usage(argv[0], 1);
    srch++;
  }

  match[0] = '\0';
  word[0] = '\0';

  while((ci = fgetc(stdin)) != EOF)
  {
    if ( ci & 0x80 )
      while ( (ci<<=1) & 0x80 ) fgetc(stdin);
    else if ( ci == '\n' || ci == '\r' )
    {
      if ( *word != '\0' && finding == 1 )
        fprintf(stdout, "%s\n", word);
      srch = match;
      while(*srch != '\0')
      {
        if ( *srch == '.' ) wild++;
        else count[*srch - 'a']++;
        srch++;
      }
      match[0] = '\0';
      word[0] = '\0';
      pos = 0;
      finding = 1;
    }
    else if ( finding == 1 )
    {
      if ( ci >= 'A' && ci <= 'Z' ) ci += 'a' - 'A';
      if ( ci >= 'a' && ci <= 'z' )
      {
        if ( count[ci - 'a'] > 0 ) 
        {
          count[ci - 'a']--;
          match[pos] = ci;
        }
        else if ( wild > 0 )
        {
          wild--;
          match[pos] = '.';
        }
        else finding = 0;
        if ( finding == 1 ) 
        {
          if ( pos < 15 )
          {
            word[pos++] = ci;
            word[pos] = '\0';
            match[pos] = '\0';
          }
          else finding = 0;
        }
      }
      else finding = 0;
    }
  }

  return 0;
}
