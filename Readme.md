# T1 — RSA simples (C / GMP)

Projeto em C que gera chaves RSA, encripta e decripta arquivos em blocos usando GMP.

Pré-requisitos
- gcc (C11), make
- libgmp-dev

Compilar
```bash
make
```
Binários vão para `.bin/`.

Uso
- Gerar chaves:
  ```bash
  .bin/main genkey [--key <arquivo_chaves>]
  ```
  - Default: `files/keys.txt`.

- Encriptar:
  ```bash
  .bin/main encrypt --in <arquivo_entrada> [--key <arquivo_chaves>] [--out <arquivo_saida>]
  ```
  - `--in` é obrigatório.
  - Default key: `files/keys.txt`
  - Default out: `files/crypt_out.txt`
  - `--key`, `--in` e `--out` não podem ser o mesmo caminho entre si.

- Decriptar:
  ```bash
  .bin/main decrypt --in <arquivo_cifrado> [--key <arquivo_chaves>] [--out <arquivo_saida>]
  ```

Formato do arquivo de chaves (funções `save_keys` / `load_keys`)
- Arquivo texto, 4 linhas (cada linha termina em `\n`):
  1. `RSA`
  2. `n` (hexadecimal)
  3. `e` (hexadecimal)
  4. `d` (hexadecimal)

Notas importantes sobre criptografia
- Tamanho do bloco: `mod_bytes = (mpz_sizeinbase(n,2)+7)/8`.
- O projeto grava cada bloco de ciphertext com tamanho fixo `mod_bytes` (left-pad com zeros à esquerda) para preservar leading zeros.
- Ao decifrar, lê-se blocos fixos `mod_bytes`, decifra e escreve somente o payload real (usando o campo `length` do packet).
- Usa modos binários (`rb`/`wb`) ao processar arquivos (para imagens, etc).

Estrutura do repositório
- `include/` — headers públicos
- `src/` — código-fonte
- `files/` — chaves, entradas e saídas de exemplo
- `.bin/` — binários gerados
- `Makefile` — build/targets

Exemplos básicos
```bash
# gerar chaves (para files/keys.txt)
.bin/main genkey

# gerar chaves com arquivo diferente
.bin/main genkey --key chaves.txt

# gerar chaves usando short flags (se aplica para outras flags)
.bin/main genkey -k chaves.txt

# encriptar arquivo de 16KiB
.bin/main encrypt --in files/arquivo_16k.txt --out files/encrypted.bin

# encriptar usando arquivo de chaves diferente
.bin/main encrypt --key files/chaves.txt --in files/arquivo_16k.txt --out files/encrypted.bin

# decriptar
.bin/main decrypt --in files/encrypted.bin --out files/decrypted.bin
```