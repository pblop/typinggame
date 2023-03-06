import string

#for l in string.ascii_lowercase:
#  print(f"KEY_{l.upper()} = {ord(l)},")

for l in string.ascii_lowercase:
  print(f"case '{l}': case '{l.upper()}': return KEY_{l.upper()};")


