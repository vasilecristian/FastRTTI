This project represent a better way to have RTTI in your code. <br>
Pros: faster, light, avoid to use standard rtti. <br>
Cons: each class must be derived from a template, each class have a little bit more size.<br>
<br>
New:<br>
A smart pointer implementation. The features are:<br>
- can replace static cast and dynamic cast based on fast-rtti.<br>
- the polymorphic mechanism can be used very easy because of fast-rtti. So you can use a list of smart pointers of a base type.<br>
<br>
<br>
The tests demonstrates that the fast-rtti is from 5 to 9 times faster than classic RTTI mechanism.