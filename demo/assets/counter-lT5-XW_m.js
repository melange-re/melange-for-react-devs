import{r as y,j as d,c as m}from"./client-DyySc-ew.js";function j(e){return e===void 0?{MEL_PRIVATE_NESTED_SOME_NONE:0}:e!==null&&e.MEL_PRIVATE_NESTED_SOME_NONE!==void 0?{MEL_PRIVATE_NESTED_SOME_NONE:e.MEL_PRIVATE_NESTED_SOME_NONE+1|0}:e}const g=function(s,t){var r=Error.call(this,s,{cause:t});return r.cause==null&&Object.defineProperty(r,"cause",{configurable:!0,enumerable:!1,writable:!0,value:t}),Object.defineProperty(r,"name",{configurable:!0,enumerable:!1,writable:!0,value:"MelangeError"}),r};g.prototype=Error.prototype;function _(e,s,t){const r=new Array(t);let n=0,c=s;for(;n<t;)r[n]=e[c],n=n+1|0,c=c+1|0;return r}function b(e,s){for(;;){const t=s,r=e,n=r.length,c=n===0?1:n,a=t.length,i=c-a|0;if(i===0)return r.apply(null,t);if(i>=0)return function(u){return b(r,t.concat([u]))};s=_(t,c,-i|0),e=r.apply(null,_(t,0,c))}}function E(e,s){const t=e.length;if(t===1)return e(s);switch(t){case 1:return e(s);case 2:return function(r){return e(s,r)};case 3:return function(r,n){return e(s,r,n)};case 4:return function(r,n,c){return e(s,r,n,c)};case 5:return function(r,n,c,a){return e(s,r,n,c,a)};case 6:return function(r,n,c,a,i){return e(s,r,n,c,a,i)};case 7:return function(r,n,c,a,i,u){return e(s,r,n,c,a,i,u)};default:return b(e,[s])}}function v(e){switch(e){case 0:return 8;case 1:return 16;case 2:return 10}}function C(e){return e>=65&&e<=90||e>=192&&e<=214||e>=216&&e<=222?e+32|0:e}function S(e){const s=e.length;if(s>31)throw new g("Invalid_argument",{MEL_EXN_ID:"Invalid_argument",_1:"format_int: format too long"});let t={justify:"+",signstyle:"-",filter:" ",alternate:!1,base:2,signedconv:!1,width:0,uppercase:!1,sign:1,prec:-1,conv:"f"},r=0;for(;;){const n=r;if(n>=s)return t;const c=e.charCodeAt(n);let a=0;if(c>=69)if(c>=88)if(c>=121)a=1;else switch(c){case 88:t.base=1,t.uppercase=!0,r=n+1|0;continue;case 101:case 102:case 103:a=5;break;case 100:case 105:a=4;break;case 111:t.base=0,r=n+1|0;continue;case 117:t.base=2,r=n+1|0;continue;case 89:case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:case 104:case 106:case 107:case 108:case 109:case 110:case 112:case 113:case 114:case 115:case 116:case 118:case 119:a=1;break;case 120:t.base=1,r=n+1|0;continue}else if(c>=72)a=1;else{t.signedconv=!0,t.uppercase=!0,t.conv=String.fromCharCode(C(c)),r=n+1|0;continue}else switch(c){case 35:t.alternate=!0,r=n+1|0;continue;case 32:case 43:a=2;break;case 45:t.justify="-",r=n+1|0;continue;case 46:t.prec=0;let i=n+1|0;for(;function(){const u=e.charCodeAt(i)-48|0;return u>=0&&u<=9}();)t.prec=(Math.imul(t.prec,10)+e.charCodeAt(i)|0)-48|0,i=i+1|0;r=i;continue;case 33:case 34:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 44:case 47:a=1;break;case 48:t.filter="0",r=n+1|0;continue;case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:a=3;break;default:a=1}switch(a){case 1:r=n+1|0;continue;case 2:t.signstyle=String.fromCharCode(c),r=n+1|0;continue;case 3:t.width=0;let i=n;for(;function(){const u=e.charCodeAt(i)-48|0;return u>=0&&u<=9}();)t.width=(Math.imul(t.width,10)+e.charCodeAt(i)|0)-48|0,i=i+1|0;r=i;continue;case 4:t.signedconv=!0,t.base=2,r=n+1|0;continue;case 5:t.signedconv=!0,t.conv=String.fromCharCode(c),r=n+1|0;continue}}}function M(e,s){const t=e.justify,r=e.signstyle,n=e.filter,c=e.alternate,a=e.base,i=e.signedconv,u=e.width,w=e.uppercase,p=e.sign;let l=s.length;i&&(p<0||r!=="-")&&(l=l+1|0),c&&(a===0?l=l+1|0:a===1&&(l=l+2|0));let o="";if(t==="+"&&n===" ")for(let f=l;f<u;++f)o=o+n;if(i&&(p<0?o=o+"-":r!=="-"&&(o=o+r)),c&&a===0&&(o=o+"0"),c&&a===1&&(o=o+"0x"),t==="+"&&n==="0")for(let f=l;f<u;++f)o=o+n;if(o=w?o+s.toUpperCase():o+s,t==="-")for(let f=l;f<u;++f)o=o+" ";return o}function N(e,s){if(e==="%d")return String(s);const t=S(e);let n=(s<0?t.signedconv?(t.sign=-1,-s>>>0):s>>>0:s).toString(v(t.base));if(t.prec>=0){t.filter=" ";const c=t.prec-n.length|0;c>0&&(n="0".repeat(c)+n)}return M(t,n)}function x(e){return N("%d",e)}function A(e){const s=y.useState(function(){return 0}),t=s[1];return d.jsxs("div",{children:[d.jsx("button",{children:"-",onClick:function(r){E(t,function(n){return n-1|0})}}),d.jsx("span",{children:x(s[0])}),d.jsx("button",{children:"+",onClick:function(r){E(t,function(n){return n+1|0})}})],style:{display:"flex",padding:"1em",gridGap:"1em"}})}const O=A;function k(e){return d.jsxs("div",{children:[d.jsx("h1",{children:"Counter"}),d.jsx(O,{})]})}const h=document.querySelector("#root");h==null?console.error("Failed to start React: couldn't find the #root element"):m(h).render(d.jsx(k,{}));h==null||j(h);