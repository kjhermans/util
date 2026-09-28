#!/usr/bin/perl

srand(time());

my $hash = {};

print "
void put_data()
{";

for (my $i=0; $i < 2048; $i++) {
  my $kl = int(rand(10)) + 2;
  my $vl = int(rand(10)) + 12;
  my $key = '';
  my $val = '';
  while ($kl--) {
    $key .= ('a'..'z')[ rand(26) ];
  }
  while ($vl--) {
    $val .= ('a'..'z')[ rand(26) ];
  }
  $hash->{$key} = $val;
  print "
  {
    vec_t key = vec_string(\"$key\"); 
    vec_t val = vec_string(\"$val\"); 
    if (hash_put(&h, &key, &val)) { fprintf(stderr, \"Put error.\\n\"); exit(-1); }
    fprintf(stderr, \"$key -> $val\\n\");
  }";
}

print "}

void get_data()
{";

for (my $i=0; $i < 2048; $i++) {
  my @keys = keys(%{$hash});
  my $key = $keys[ int(rand(scalar(@keys))) ];
  my $val = $hash->{$key};
  print "
  {
    vec_t key = vec_string(\"$key\");
    vec_t val = { 0 };
    if (hash_get(&h, &key, &val) == 0) {
      if (0 == strcmp(val.data, \"$val\")) {
        fprintf(stderr, \"Retrieval $key returns ok.\\n\");
        free(key.data);
      } else {
        fprintf(stderr, \"Retrieval $key returns '%-.*s'\\n\", get.size, get.data);
        exit(-1);
      }
    } else {
      fprintf(stderr, \"Retrieval $key returns non zero.\\n\");
      exit(-1);
    }
  }";
}

print "
}
";

1;
