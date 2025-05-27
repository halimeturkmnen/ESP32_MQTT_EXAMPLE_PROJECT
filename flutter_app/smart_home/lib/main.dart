import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';

final Future<FirebaseApp> _initialization = Firebase.initializeApp();

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Home App',
      theme: ThemeData(primarySwatch: Colors.blue),
      home: FutureBuilder(
        future: _initialization,
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.done) {
            return const HomePage();
          } else if (snapshot.hasError) {
            return const Center(child: Text('There is an error on Firebase'));
          }
          return const Center(child: CircularProgressIndicator());
        },
      ),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _firestore = FirebaseFirestore.instance;

  Future<void> _sendCommand(String documentId, String action) async {
    await _firestore.collection('commands').doc(documentId).set({
      'command': action,
      'timestamp': FieldValue.serverTimestamp(),
    });
  }

  @override
  Widget build(BuildContext context) {
    CollectionReference esp32Ref = _firestore.collection('esp32');

    return Scaffold(
      backgroundColor: Colors.grey,
      appBar: AppBar(title: const Text('My Smart Home')),
      body: Center(
        child: Container(
          child: Column(
            children: [
              Flexible(
                child: StreamBuilder<QuerySnapshot>(
                  stream: esp32Ref.snapshots(),
                  builder: (BuildContext context, AsyncSnapshot asyncSnapshot) {
                    if (asyncSnapshot.hasError) {
                      return const Text('There is an error');
                    }
                    if (asyncSnapshot.connectionState ==
                        ConnectionState.waiting) {
                      return const Center(child: CircularProgressIndicator());
                    }
                    List<DocumentSnapshot> listOfDocumentSnap =
                        asyncSnapshot.data.docs;
                    return ListView.builder(
                      itemCount: listOfDocumentSnap.length,
                      itemBuilder: (context, index) {
                        String documentId = listOfDocumentSnap[index].id;
                        Map<String, dynamic> data =
                            listOfDocumentSnap[index].data()
                                as Map<String, dynamic>;
                        String value = '${data['value']}';

                        return Card(
                          child: ListTile(
                            title: Text(
                              documentId,
                              style: const TextStyle(fontSize: 24),
                            ),
                            subtitle: Column(
                              crossAxisAlignment: CrossAxisAlignment.start,
                              children: [
                                Text(
                                  value,
                                  style: const TextStyle(fontSize: 16),
                                ),
                                Row(
                                  children: [
                                    IconButton(
                                      icon: const Icon(
                                        Icons.add,
                                        color: Colors.green,
                                      ),
                                      onPressed: () {
                                        _sendCommand(documentId, 'increase');
                                      },
                                    ),
                                    IconButton(
                                      icon: const Icon(
                                        Icons.remove,
                                        color: Colors.red,
                                      ),
                                      onPressed: () {
                                        _sendCommand(documentId, 'decrease');
                                      },
                                    ),
                                  ],
                                ),
                              ],
                            ),
                          ),
                        );
                      },
                    );
                  },
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
