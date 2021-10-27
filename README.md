# CacheInfo
Попытка узнать длину кэш линии и размеры уровней кэшей.
## Сache Line Size 
Мы бегаем по массиву char-ов с шагом (stride) от 8 до 4096 байт. 

При этом мы пытались обойти сache prefetching, поэтому добавили рандомизацию. Смотрим каждый n-ый элемент не в порядке очередности, а в перемешанном.

У нас два подхода к количеству чтений элементов массива:

1. размер массива / размер шага * factor, где factor - определенная константа, задающая сколько раз мы обходим массив.
2. фиксированное количество чтений

В первом подходе мы ожидали, что пока stride помещается в длину кэш линии, мы должны получать примерно одинаковое время на проход по массиву. А как только stride станет больше длины линии, время прохода должно начать резко уменьшаться. Конкретно, мы каждый раз увеличиваем stride в два, поэтому каждый раз количество чтений уменьшается в два раза. При этом если stride меньше размера кеш линии, то количество кеш мисов должно получаться одинаковым. Если же stride больше кеш линии, то количество кеш мисов должно равняться количеству чтений.

Во втором подходе, так как число чтений фиксировано, после выхода за длину кэш линии время наоборот должно увеличиться.
## Сache Size 
В предположении, что мы узнали длину кэш линии, мы фиксируем stride (например, 64).
Далее, мы начинаем итерироваться по размеру массива от длины линии до 64 Мбайт. Для каждой итерации производим (размер массива / размер шага) * factor чтений.

Мы ожидали, что при переходах между уровнями кэша будут появляться большое скачки во времени.

## Project structure
### loads
Функция подсчета времени прохода по массиву

Принимает
1. size - размер массива
2. stride - размер шага
3. factor - константа (мы использовали 10)
4. iterCount - константа, используется для усреднения результата
5. factory - вспомогательная структура для генерации массива (по дефолту массив генерируется так, чтобы мы обходили все его элеметы в рандомном порядке)
6. isConstIterations - флаг, переключатель подходов к количеству чтений
  1. true - константное
  2. false - size/stride * factor
8. unit - единицы измерения времени
### сacheLineSize.cpp/cacheSize.cpp
Выводят результаты проходов по массиву для вычисления длины кэш линии/размеров уровней кэша

Вид: размер шага/массива | время | текущее время/время на предыдущем шаге

## Результаты 

### MacOS, процессор Intel Core i9-9880H @ 2.30GHz 
+ Длина линейки: 64 Б
+ L1: 32 КБ
+ L2: 256 КБ
+ L3: 16 МБ

#### cacheLineSize.cpp: 
1. isConstIterations = false  
  16:   57088  0.622152  
  32:   41619  0.729032  
  64:   40159  0.96492  
  128:  49338  1.22857  
  256:  50595  1.02548  
  ***512:  32431  0.640992***  
  1024: 20027  0.617526  
  2048: 14047  0.701403  
  4096: 7104   0.505731  
  Получаем, что время уменьшается примерно в два раза, начиная с stride=512, значит длина кэш линии = 256, что неправда
2. isConstIterations = true  
  16:   113614   1.23703  
  32:   128973   1.13519  
  64:   271279   2.10338  
  ***128:  612495   2.2578***  
  256:  1239849  2.02426  
  512:  1436012  1.15822  
  1024: 1856858  1.29307  
  2048: 2539189  1.36747  
  4096: 2659134  1.04724  
  Тут почти получили нужный результат в 64 Б, но и после 32-х сильный скачек
  
#### cacheSizes.cpp:
128:      109       1.29762  
256:      167       1.53211  
512:      278       1.66467  
1024:     501       1.80216  
2048:     944       1.88423  
4096:     1829      1.9375  
8192:     3604      1.97048  
16384:    7157      1.98585  
32768:    15179     2.12086  
***65536:    46833     3.08538***  
131072:   93652     1.9997  
***262144:   257231    2.74667***  
524288:   549218    2.13512  
1048576:  1059242   1.92864  
2097152:  2219739   2.09559  
4194304:  4988209   2.24721  
***8388608:  15899308  3.18738***  
16777216: 21061159  1.32466  
***33554432: 56366745  2.67634***  

Получаем нужный скачок для L1: 32 КБ  
А вот с L2  не повезло: получили скачек после 128 КБ  
Потом почему-то после 4 МБ  
И уже после нужных нам 16МБ

### Ubuntu, процессор AMD Ryzen 7 3750H
+ Длина линейки: 64 Б
+ L1: 16 КБ
+ L2: 2 МБ
+ L3: 4 МБ

#### cacheLineSize.cpp:
1. isConstIterations = false  
  16:   63680  0.520342  
  32:   39860  0.625942  
  64:   30689  0.76992  
  128:  24837  0.809313  
  256:  18960  0.763377  
  512:  11289  0.595411  
  1024: 6105   0.540792  
  2048: 3460   0.566749  
  4096: 2047   0.591618  
  Все очень странно, скачков вообще нет
2. isConstIterations = true  
  16:   100644  1.06115  
  32:   123529  1.22739  
  64:   186859  1.51267  
  128:  282270  1.5106  
  256:  472603  1.67429  
  512:  554108  1.17246  
  1024: 618736  1.11663  
  2048: 690152  1.11542  
  4096: 839206  1.21597  
  И тут тоже

#### cacheSizes.cpp:
128:      68       1.19298  
256:      126      1.85294  
512:      235      1.86508  
1024:     456      1.94043  
2048:     896      1.96491  
4096:     1778     1.98438  
8192:     3542     1.99213  
16384:    7110     2.00734  
32768:    14212    1.99887  
65536:    28288    1.99043  
131072:   56537    1.99862  
262144:   113170   2.0017  
524288:   229617   2.02896  
1048576:  460955   2.0075  
2097152:  1022418  2.21804  
***4194304:  4942021  4.83366***  
8388608:  9257840  1.87329  
16777216: 17653612 1.90688  
33554432: 32649621 1.84946

Время увеличивается пропорционально размеру массива, кроме скачка после 2 МБ, что соответствует L2




