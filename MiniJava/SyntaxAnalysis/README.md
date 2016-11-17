### Построение AST (Abstract Syntax Tree)  

Последовательность команд:  

```bash
# генерирует парсер по грамматике, собирает решение
make

# парсит файлы из директории data/Samples/, строит AST, сохраняет в формате DOT в data/SamplesGV/
./generateGVs.py
# или
./generateGVs.py -i data/Samples -o data/SamplesGV

# компилирует файлы (DOT формат) в картинки (SVG), сохраняет в data/SamplesAST/
./generateImages.py
# или
./generateImages.py -i data/SamplesGV -o data/SamplesAST
```

При желании можно включить подробный вывод у PrintVisitor'а. Для этого замените в вызове `traverseAST` второй аргумент с `false` на `true`:
```c++
std::string traversal = traverseAST( astRoot.get(), true );
```  

Качать актуальную версию flex отсюда: https://sourceforge.net/projects/flex/files/
