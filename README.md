# GetNextLine

## English

A C function that retrieves one line at a time from a text file or terminal input. Repeated calls on the same input return successive lines.

For input `first\nsecond`, it returns `"first\n"`, then `"second"`, then `NULL` after the input is exhausted. Here `\n` represents a newline. The input is identified by a file descriptor (fd), the integer returned by functions such as `open()`.

### Single-file and multiple-file versions

| Sources | Behavior |
| --- | --- |
| `get_next_line.c` + `get_next_line_utils.c` | Read successive lines from one fd |
| `get_next_line_bonus.c` + `get_next_line_utils_bonus.c` | Keep separate unread data when alternating between files |

### Build and try

Requires a C compiler and Make. The supplied [main.c](main.c) reads standard input or a file given as an argument.

```sh
make
printf 'first line\nsecond line' | ./gnl_example
./gnl_example README.md
make bonus
./gnl_bonus_example README.md
```

The example prints each retrieved line to the terminal. You can also use `make run ARGS=README.md`. Set the read size with a command such as `make re BUFFER_SIZE=1024`.

### Implementation

- `BUFFER_SIZE` is the maximum number of bytes requested by one `read` call; the default is 42.
- Returned lines include their newline when present. The function returns `NULL` at end of file with no unread data, or on error.
- The caller must `free` each returned string.

### Tests

```sh
make test
```

Checks empty input, newlines, long lines, EOF, invalid fds, and interleaved files in the bonus version with buffer sizes 1, 42, and 1024.

## 日本語

テキストファイルや端末の入力を、1行ずつ取り出すC関数です。同じ読み込み先に対して繰り返し呼ぶと、次の行、その次の行と順に文字列を返します。

例えば内容が `first\nsecond` なら、最初の呼び出しは `"first\n"`、次は `"second"`、読み終えた後は `NULL` を返します。ここで `\n` は改行です。読み込み先は、`open()` などが返す整数の識別番号（ファイルディスクリプタ、fd）で指定します。

### 通常版と複数ファイル対応版

| ソース | 動作 |
| --- | --- |
| `get_next_line.c` + `get_next_line_utils.c` | 1つのファイルディスクリプタから順に読み込む |
| `get_next_line_bonus.c` + `get_next_line_utils_bonus.c` | 複数のファイルを交互に読んでも、それぞれの続きから返す |

### ビルドと使用例

Cコンパイラ・Make が必要です。付属の [main.c](main.c) は、標準入力または引数で指定したファイルを1行ずつ読み込みます。

```sh
make
printf 'first line\nsecond line' | ./gnl_example
./gnl_example README.md
make bonus
./gnl_bonus_example README.md
```

この実行例は、取り出した行を順に端末へ表示します。`make run ARGS=README.md` でも実行できます。読み込みサイズは `make re BUFFER_SIZE=1024` のように指定します。

### 実装メモ

- `BUFFER_SIZE` は1回の `read` で読み込む最大バイト数です。省略時は42です。
- 戻り値には改行があればその改行も含まれます。ファイルの終わり（EOF）まで読み、未読データがない場合、またはエラー時は `NULL` を返します。
- 返された文字列は呼び出し側で `free` します。

### テスト

```sh
make test
```

BUFFER_SIZEが1・42・1024の場合の空入力、改行、長い行、EOF、不正なfd、ボーナス版の複数fdを確認します。
