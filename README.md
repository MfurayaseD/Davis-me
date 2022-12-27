# Davis-me
The code in this repository gives a user 4 options:
1. Store the contents of user file to another file called system.txt
2. Censor certain words that are chosen by the user in the user file and stores the result in system.txt
3. Encrypts the contents of the user file and stores the result in system.txt
4. Decrypts the contents of system.txt

Two files with the names system.txt and user.txt should be stored together with the main code in one folder to get the expected output.
The encryption used is columnar transposition, a technique for encryption used by the US Army in world war I.

Columnar Transposition works by taking a key. The key is used for both encryption and decryption. For example:
let's say the key chosen is "HACK" and the message to be transferred is "Attack at dawn"

To encrypt we put both the key and the message in a matrix. (As you may have observed there are whitespaces, we can replace them with "-" for instance)
The matrix would look like:

  H A C K

  A t t a

  c k - a

  t - d a

  w n - -

we then re-organising the key in alphabetical order the key while maintaining the columns.

  A C H K

  t t A a

  k - c a

  - d t a

  n - w -

we then read off the matrix column by column.
The cipher text would be: tk-nt-d-Actwaaa-

Decryption is just the complete reverse of the above. Re-organise the columns with the sorted key to spell the original key and read off row by row.
