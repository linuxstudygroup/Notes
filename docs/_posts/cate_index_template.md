---
layout: default
title: "目录"
categories: "cate1"
---
<ul>
{% for page in site.categories.cate1 %}
    <li>
        <a href="{{ page.url }}">{{ page.title }}</a>
    </li>
{% endfor %}

</ul>
