# Hangman

## How to play

- register word

```
cleos push action <deployed account> registerword ["hoge"] -p <deployed account>@<permission>
```

- start playing

```
cleos push action <deployed account> start '["<player account>"]' -p <player account>@<permission>
```

- try to answer

```
cleos push action <deployed account> submit '["<game id>", "<alphabet>"]' -p <player account>@<permission>
```
