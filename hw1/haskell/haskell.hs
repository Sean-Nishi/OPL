//attempt at explaining haskell code from http://rosettacode.org/wiki/Combinations#Haskell
comb :: Int -> [a] -> [[a]]
comb 0 _      = [[]] -- a list containing an empty list.
comb _ []     = [] --it doesn't matter what we draw from the list but this will equal an empty list
-- : is the cons operator and that puts something at the beginning of a list
comb m (x:xs) = map (x:) (comb (m-1) xs) ++ comb m xs --x:xs takes an item and puts it in a list m is the parameter map transforms x into a list
