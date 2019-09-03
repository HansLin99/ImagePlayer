# ImagePlayer
***Overview***:
We used a simplified PNG class to build this program. Inside the program, we manipulated pixels using different data structures and re-color the image to make a different look. Finally, we learn about lossy compression to PNG.

## Part 1:
In this part, we cut PNG into several rectangles and put them into nodes in a chain structure(linked list). 

- We changed the sequence of the nodes to re-assemble the image.
- We changed the color of some block.
- **We changed the color of some special pixels to make a watermark (a small try in lab)**

![Sequence Changed](https://github.com/HansLin99/ImagePlayer/blob/master/Image%20Player%20Part%201/images/given-moveBack.png "UBC Rose Garden")
![Special Block Color Changed](https://github.com/HansLin99/ImagePlayer/blob/master/Image%20Player%20Part%201/images/given-blocktest.png)
![Watermark](https://github.com/HansLin99/ImagePlayer/blob/master/files/out-watermark%20copy.png)

\*You can find all the output images in the ***images*** folder.


## Part 2:
In this part, we try to implement a color flood algorithm.   

![BFS](https://github.com/HansLin99/ImagePlayer/blob/master/files/bfsraingirl.gif "BFS")
![DFS](https://github.com/HansLin99/ImagePlayer/blob/master/files/dfsraingirl.gif "DFS")

We created several color spread pattern and used BFS/DFS to fill the color into each pixel.

\*You can find all the output images in the ***images*** folder.

## Part 3:

In this part, we try to implement simple lossy compression on PNG image.
![Lossy Compression](https://github.com/HansLin99/ImagePlayer/blob/master/files/PA3pic1.png "Original")
![Lossy Compression](https://github.com/HansLin99/ImagePlayer/blob/master/files/outputPA3pic1.png "Output")

---

We cut the image into several small pieces and put them into nodes in a quad-tree structure(4 branches tree).
