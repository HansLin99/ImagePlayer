# ImagePlayer
***Overview***:
We used a simplified PNG class to build this program. Inside the program, we manipulated pixels using different data structures and re-color the image to make a different look. Finally, we learn about lossy compression to PNG.

## Part 1:
In this part, we cut PNG into several rectangles and put them into nodes in a chain structure(linked list). 

- We changed the sequence of the nodes to re-assemble the image.
- We changed the color of some block.
- **We changed the color of some special pixels to make a watermark (small try in lab)**

![Sequence Changed](https://github.com/HansLin99/ImagePlayer/blob/master/Image%20Player%20Part%201/images/given-moveBack.png "UBC Rose Garden")
![Special Block Color Changed](https://github.com/HansLin99/ImagePlayer/blob/master/Image%20Player%20Part%201/images/given-blocktest.png)
![Watermark](https://github.com/HansLin99/ImagePlayer/blob/master/files/out-watermark%20copy.png)

\*You can find all the output images in the ***images*** folder.


## Part 2:

Description:
Color flood algorithm using BFS/DFS and deque data structure.

## Part 3:

Description:
Lossy compression using a quadTree structure. The image is cut into pieces based on minimal entropy of splitting point.

