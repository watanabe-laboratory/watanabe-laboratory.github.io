# diststep.rb
# 全頂点間最短距離の計算（毎ステップで止める）

### 各種サブルーチンの定義 #############
# min 小さい方を求めるサブルーチン
# -1 は「無限大」として特別扱いする
def min(x, y)
   if x == -1
      return(y)
   elsif y == -1
      return(x)
   else
      if x < y
         return(x)
      else
         return(y)
      end
   end
end

# 二辺の和を求めるサブルーチン
# -1 は「無限大」として特別扱いする
def plus(x, y)
   if x == -1 || y == -1
      return(-1)
   else
      return(x + y)
   end
end

# 行列の出力のためのサブルーチン
def printmatrix(n, d)
   for i in 0..n-1
      for j in 0..n-1
         printf("%3d, ", d[i][j])
      end
      print("\n")
   end
   print("\n")
end

# 距離行列のための行列積計算サブルーチン
# U * V
def mult(n, u, v)
   w = Array.new(n){ Array.new(n) }  # 計算途中結果保持用の配列
   for i in 0..n-1
      for j in 0..n-1
         tmp = -1
         for k in 0..n-1
            x = plus(u[i][k], v[k][j])
            tmp = min(tmp, x)
         end
         w[i][j] = tmp
      end
   end
   return(w)
end

############################

# サイズ n の設定と準備
n = gets().to_i
a = Array.new(n){ Array.new(n) }
d = Array.new(n){ Array.new(n) }

# 隣接距離行列入力
for i in 0..n-1
   a[i] = gets().split.map(&:to_i)
end
printmatrix(n, a)  # 入力データ出力

# 行列 d の計算
# 1 ステップ目
d = a
print("t = ", 1, "\n")
printmatrix(n, a)
sleep(2)

# 繰り返し
t = 1
while t < n
   t = t + 1
   # t ステップ目の計算
   d = mult(n, d, a)    # D = D * A
   print("t = ", t, "\n")
   printmatrix(n, d)
   sleep(2)
end

# 答えの出力
print("result\n")
printmatrix(n, d)
