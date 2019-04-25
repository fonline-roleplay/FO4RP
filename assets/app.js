function textNodesUnder(node){
    var all = [];
    for (node=node.firstChild;node;node=node.nextSibling){
        if (node.nodeType==3) all.push(node);
        else all = all.concat(textNodesUnder(node));
    }
    return all;
}

// IE and some browsers tend to escape all html inside of noscript tag so .text() should be used instead of .html()
var noscriptSwap = ($("<noscript><div>OK</div></noscript>").html().indexOf("&gt;") == 7)

$.fn.isInViewport = function() {
    var elementTop = $(this).offset().top;
    var elementBottom = elementTop + $(this).outerHeight();
    var viewportTop = $(window).scrollTop();
    var viewportBottom = viewportTop + $(window).height();
    return elementBottom > viewportTop && elementTop < viewportBottom;
};

var exTerminal = undefined
var anims = []
var speed = 1
var gen = function(node,text){return function(){node.data = text}}
function typeIn(el) {
    var nodes = textNodesUnder(el)
    for (var i = 0; i < nodes.length; i++) if(!nodes[i].done) {
        nodes[i].done = true
        data = nodes[i].data.split('')
        var noise = nodes[i].data.replace(/[^\s]/gi," ")
        nodes[i].data = noise
        for (var pos = 0; pos < data.length; pos+=speed)
            anims.push(gen(nodes[i],data.slice(0,pos+1).join('') + noise.substr(0,data.length - pos - 1)))
        anims.push(gen(nodes[i],data.join('')))
    }
}
setInterval( function(){ 
    if (anims.length) {
        anims.shift()() 
        if (Math.random() < 0.2) createjs.Sound.play("scroll")
        //if (exTerminal) exTerminal.pause()
    } else {
        //if (exTerminal) exTerminal.resume()
    }
    speed -= 10
    if (speed < 0 ) speed = 0
},0)

if (createjs) {
    createjs.Sound.registerSound("/assets/sfx/login.mp3", "login");
    createjs.Sound.registerSound("/assets/sfx/logout.mp3", "logout");
    createjs.Sound.registerSound("/assets/sfx/scroll.mp3", "scroll");
    createjs.Sound.registerSound("/assets/sfx/select.mp3", "select");
    createjs.Sound.registerSound("/assets/sfx/select2.mp3", "select2");

    createjs.Sound.volume = 0.25

    $(window).bind('beforeunload', function(){
        createjs.Sound.play("logout")
    });
}

jQuery(function($){
    $("body").append("<div id='terminal' class='tbody'></div>");

    $(document).on('click', 'a', function(e){
        e.preventDefault()
        var href = $(this).attr('href')
        if (href.startsWith("#")) {
            if (href.startsWith("#!")) {
                if (exTerminal) exTerminal.exec(href.substr(2))
                return
            } else return
        }
        if (exTerminal) exTerminal.exec("show " + href,true)
    })

    $(document).on("mouseover","a",function(e){
        e.preventDefault()
        //
        if(!$('.cmd').isInViewport()) return;
        //
        var href = $(this).attr('href')
        if (href.startsWith("#")) {
            if (href.startsWith("#!")) {
                if (exTerminal) exTerminal.set_command(href.substr(2))
                return
            } else return
        }
        if (exTerminal) exTerminal.set_command("show " + href,true)
    })

    $(document).on("mouseleave","a",function(e){
        if (exTerminal) exTerminal.set_command("",true)
    })

    $("#terminal").terminal(function(cmd,term){
        var args = cmd.split(/\s/)
        switch(args[0]) {
            case "": return
            case "go":
            case "run":
            case "show":
            case "load":
            case "открыть":
            case "cat":
            case "!":
            case "+" :
                //term.pause()
                $.get(args[1],function(html){
                    var el = document.createElement( 'html' );
                    el.innerHTML = html;
                    var ns = el.getElementsByTagName("noscript");
                    if (ns.length > 0) {
                        if (noscriptSwap) {
                            el.innerHTML = ns[0].innerText;
                        } else {
                            el.innerHTML = ns[0].innerHTML;
                        }
                        el = $("#main",el);
                        term.echo(el.html(),{raw:true});
                        term.echo('<div class="hr">==============================================================================================================</div>',{raw:true})
                        if (history) {
                            if (args[1].startsWith("/")) {
                                history.pushState(null, html, args[1]);
                            } else if (args[1].startsWith("http")) {
                                history.pushState(null, html, args[1]);
                            } else {
                                history.pushState(null, html, "/" + args[1]);
                            }
                        }
                    } else {
                        window.open(args[1])
                    }
                }).fail(function(){
                    if (arguments[0].status !== 404) window.open(args[1])
                    else term.echo("ОШИБКА: Файл не найден")
                })
            break 
            case "log": 
            case "git": 
            case "гит": 
            case "лог": 
                $.get("https://api.github.com/repos/fonline-roleplay/FO4RP/commits", function(result) {
                    result = result.reverse();
                    for (var i in result) {
                        term.echo("<div><a href='"+result[i].html_url+"'>"+(new Date(result[i].commit.author.date).toLocaleDateString().replace(/\./g,'/'))+" - "+result[i].author.login+"</a>: "+result[i].commit.message.replace(/\n+/g,"<br>")+"</div>", {raw:true})
                    }
                    term.echo('<div class="hr">==============================================================================================================</div>',{raw:true})
                })
            break
            case "echo":
                term.echo(cmd,{raw:true})
                term.echo('<div class="hr">==============================================================================================================</div>',{raw:true})
            break
            case "menu":
            case "nav":
                var menu = $("header nav").html();
                if (!menu) {
                    term.echo("ОШИБКА: Меню не найдено");
                    return
                }
                term.echo("<nav>"+menu+"</nav>", {raw:true});
                term.echo('<div class="hr">==============================================================================================================</div>',{raw:true})
            break
            case "return":
            case "up":
                setTimeout(function(){
                    $("#terminal").scrollTop(0)
                },300)
            break
            case "reboot":
                window.location.reload(true)
            break;
            case "eval":
                var rr= cmd.substr(4)
                term.echo(eval(!!rr ? rr : "Complete"))
            break
            default: 
                term.echo("ОШИБКА: Неизвестная команда <"+args[0]+">")
                term.echo('<div class="hr">==============================================================================================================</div>',{raw:true})
            break
            case "/?":
            case "?":
            case "man":
            case "help":
                term.echo("go | run | show | load | открыть | cat <filename>")
                term.echo("echo <html>")
                term.echo("eval <js>")
                term.echo("log | git | гит | лог")
                term.echo("up | return")
                term.echo("nav | menu")
                term.echo("reboot")
                term.echo("clear")
            break
        }
    }, {
        greetings: '',
        name: 'terminal',
        prompt: 'guest@poseidon:net/> ',
        scrollOnEcho: false,
        onAfterCommand : function(){
            createjs.Sound.play(Math.random() > 0.5 ? "select" : "select2")
        },
        onInit: function(term) {
            exTerminal = term
            
            var fn = term.echo
            term.echo = function() {
                var res = fn.apply(term, arguments)
                if (arguments[0].indexOf(term.get_prompt()) < 0) {
                    speed += 10

                    $("div[data-index]").each(function(i,el){
                        $(el).removeAttr('data-index')
                        typeIn(el)
                    })
                }
                return res
            }

            if (noscriptSwap) {
                term.echo($("#source").text(),{raw:true})
            } else {
                term.echo($("#source").html(),{raw:true})
            }
            
            $("#source").remove()

            $('#terminal').unbind('scroll')

            $(".prompt").on('click',function(e){
                term.exec("nav");
            })

            if (window.Hammer) {
                var mc = new Hammer(document.body);
                mc.get('swipe').set({ direction: Hammer.DIRECTION_ALL });
                mc.on("swipeleft", function (e) { 
                    $("#terminal").scrollTop( $("#terminal").scrollTop() + e.deltaX )
                });               
                mc.on("swiperight", function (e) { 
                    $("#terminal").scrollTop( $("#terminal").scrollTop() + e.deltaX )
                });
                mc.on("swipeup", function (e) { 
                    $("#terminal").scrollTop( $("#terminal").scrollTop() + e.deltaY )
                });                 
                mc.on("swipedown", function (e) { 
                    $("#terminal").scrollTop( $("#terminal").scrollTop() + e.deltaY )
                });
            }
        }
    })

    if (createjs) {
        $('#terminal').on('scroll',function(){
            createjs.Sound.play("scroll")
        })
        setTimeout(function(){
            createjs.Sound.play("login")
            if ($("#source").length > 0) {
                $("#terminal").html( $("#source").html() );
                $("#source").remove()
            }            
        },200)
    }

    $(document).on('click','.scroll-up',function(e){
        e.preventDefault()
        $("#terminal").scrollTop(0)
    })

    $(document).on('click','.scroll-dn',function(e){
        e.preventDefault()
        $("#terminal").scrollTop($("#terminal").innerHeight()*2)
    })
})
