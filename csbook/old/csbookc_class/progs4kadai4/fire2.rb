# fire2.rb

##################
# プログラム本体 #
##################
# 初期設定
n    = ARGV[0].to_i;  n2 = 2*n + 2;  size = n2 - 1;  ntree = size * size
pint = ARGV[1].to_i;  p = pint.to_f / 100.0
bh   = ARGV[2].to_i;
th   = ARGV[3].to_i;
seed = ARGV[4].to_i;  srand seed

#printf "size = %d (# of trees = %d)\n", size, ntree
#printf "prob. = %f, burn out time = %d, total sim. time = %d, seed = %d\n", p, bh, th, seed

# 森林状態保持用
f = [];  fnext = []
for i in (0..n2) do
   f[i] = Array.new(size)
   fnext[i] = Array.new(size)
   for j in (0..n2) do
      f[i][j] = -1
   end
end
for i in (0..n2) do
   f[i][0] = f[i][n2] = 0
   f[0][i] = f[n2][i] = 0
   fnext[i][0] = 0;  fnext[i][n2] = 0
   fnext[0][i] = 0;  fnext[n2][i] = 0
end

########################
# シミュレーション開始 #
########################
f[n+1][n+1] = 1
nbtree = 1
h = 0

while true do
   newflag = false
   for i in (1..(n2-1)) do
      for j in (1..(n2-1)) do
         if f[i][j] >= bh then fnext[i][j] = 0
         elsif f[i][j] > 0 then fnext[i][j] = f[i][j] + 1
         elsif f[i][j] == 0 then fnext[i][j] = 0
         else
            # まだ木が燃えていない場合
            fnext[i][j] = -1
            endflag = false
            for di in (-1..+1) do
               for dj in (-1..+1) do
	          ii = i + di;  jj = j + dj
                  if (f[ii][jj] > 0) && ((rand * 100) < pint) then
                                         # ↑確率 p で燃え移る
                     fnext[i][j] = 1
                     endflag = true
                     break
                  end # if
               end # for
               if endflag then break end
            end # for
         end #if
      end # for
   end # for

   # 結果更新
   h = h + 1
   nubtree = nbtree = 0
   for i in (1..(n2-1)) do
      for j in (1..(n2-1)) do
         f[i][j] = fnext[i][j]
         if f[i][j] > 0 then nbtree = nbtree + 1;  newflag = true
         elsif f[i][j] == 0 then nbtree = nbtree + 1
         else # if f[i][j] == -1
            nubtree = nubtree + 1
         end
      end
   end

   # 終了条件判定
   if !newflag || (nubtree == 0) || (h >= th) then break end
end
printf "time = %d: nb.tree = %d, nub.tree = %d\n", h, nbtree, nubtree
