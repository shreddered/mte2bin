# mte2bin

Как-то раз мне стало скучно, и я пореверсил формат файла `.mte`.

Так родилась мини-утилита для преобразования программы для эмулятора _mtemu_ в файлы
для МПЗУ в ДЗ3 по АСВТ.

Запуск:
```bash
$ mte2bin <input> <output prefix>
```

Генерирует 5 `.bin` файлов с заданным префиксом.

|      ROM1    |         ROM2       |       ROM3      |        ROM4       |         ROM5        |
|--------------|--------------------|-----------------|-------------------|---------------------|
|B0-B3 + D0-D3 | A0-A3 + C0 + I0-I2 | I3-I8 + CA0-CA1 | CA2-CA3 + AR0-AR5 | AR6-AR7 + доп. поля |
